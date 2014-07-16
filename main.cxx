#include <algorithm>
#include <random>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <thread>

#include <cstdlib>
#include <csignal>
#include <cinttypes>

#include <curses.h>

#include <CL/cl.hpp>

enum class FieldState
    : std::uint8_t
{
    Dead = 0,
    Alive = 1,
};

template <template <typename...> class Container>
void init_field(Container<FieldState> & field)
{
    std::random_device rng;
    std::mt19937 engine(rng());
    std::uniform_int_distribution<> dist(0, 23);

    std::generate(std::begin(field), std::end(field), [&]() -> FieldState {
        return dist(engine) ? FieldState::Dead : FieldState::Alive;
    });
}

std::string load_kernel(std::string const & filename)
{
    std::ifstream source(filename);
    if (!source.is_open())
    {
        source.open("../" + filename);
        if (!source.is_open())
            return std::string();
    }

    return std::string(
        std::istreambuf_iterator<char>(source),
        std::istreambuf_iterator<char>()
    );
}

template <template <typename...> class Container>
void render_field(Container<FieldState> const & field)
{
    clear();

    for (auto & fs : field)
    {
        if (fs == FieldState::Alive)
            addch(' ' | A_REVERSE);
        else
            addch(' ');
    }

    refresh();
}

cl_platform_id select_platform()
{
    try
    {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        for (unsigned n = 0; n < platforms.size(); ++n)
            std::cout << (n + 1) << " - " << platforms[n].getInfo<CL_PLATFORM_NAME>() << std::endl;

        std::cout << "Select a platform: ";
        unsigned index;
        while (!(std::cin >> index) || !index || (index > platforms.size()))
        {
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
        }

        return platforms[index - 1]();
    }
    catch (cl::Error const & err)
    {
        std::cerr << "OpenCL-Error: " << err.what() << "(" << err.err() << ")" << std::endl;
        return nullptr;
    }
}

volatile bool run = true;

void sigint_handler(int)
{
    run = false;
}

int main()
{
    auto platform = select_platform();
    if (!platform)
        return EXIT_FAILURE;

    try
    {
        //////////////
        // UI setup //
        //////////////
        std::signal(SIGINT, &sigint_handler);

        auto screen = initscr();
        noecho();
        curs_set(0);

        unsigned width = getmaxx(screen);
        unsigned height = getmaxy(screen);
        std::vector<FieldState> field(width * height);
        init_field(field);

        // update twice per second
        auto const timeout = std::chrono::milliseconds(500);

        //////////////////
        // OpenCl-Setup //
        //////////////////
        cl_context_properties cprops[] = {
            CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
            0,
        };

        cl::Context ctx(CL_DEVICE_TYPE_ALL, cprops, nullptr, nullptr);
        cl::Buffer in_buffer(ctx, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, field.size(), field.data());
        cl::Buffer out_buffer(ctx, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, field.size(), field.data());

        std::string source(load_kernel("rules.cl"));
        if (source.empty())
            throw std::runtime_error("Could not load rules.cl!");

        cl::Program::Sources sources = {
            { source.c_str(), source.length() + 1 },
        };
        cl::Program program(ctx, sources);

        auto devices = ctx.getInfo<CL_CONTEXT_DEVICES>();
        program.build(devices);

        cl::Kernel kernel(program, "GameOfLife");
        kernel.setArg(0, in_buffer);
        kernel.setArg(1, out_buffer);
        kernel.setArg(2, width);
        kernel.setArg(3, height);

        cl::CommandQueue queue(ctx, devices[0]);

        ///////////////
        // main loop //
        ///////////////
        std::chrono::system_clock::time_point start;
        while (run)
        {
            if (std::chrono::system_clock::now() - start > timeout)
            {
                // update the field
                cl::Event event;
                queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(field.size()), cl::NDRange(1, 1), nullptr, &event);
                event.wait();

                queue.enqueueReadBuffer(out_buffer, CL_TRUE, 0, field.size(), field.data());
                queue.enqueueCopyBuffer(out_buffer, in_buffer, 0, 0, field.size(), nullptr, &event);
                event.wait();

                render_field(field);

                start = std::chrono::system_clock::now();
            }
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        /////////////
        // cleanup //
        /////////////
        endwin();
        return EXIT_SUCCESS;
    }
    catch (cl::Error const & err)
    {
        endwin();
        std::cerr << "OpenCL-Error: " << err.what() << "(" << err.err() << ")" << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception const & ex)
    {
        endwin();
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
