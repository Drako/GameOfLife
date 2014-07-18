__constant sampler_t SAMPLER = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE;

uint cell(__read_only image2d_t in, int x, int y)
{
    int const width = get_image_width(in);
    int const height = get_image_height(in);

    if (x == -1) x = width - 1;
    if (y == -1) y = height - 1;
    if (x == width) x = 0;
    if (y == height) y = 0;

    uint4 state = read_imageui(in, SAMPLER, (int2)(x, y));
    return state.s3;
}

__kernel void GameOfLife(__read_only image2d_t in, __write_only image2d_t out)
{
    int x = get_global_id(1);
    int y = get_global_id(0);

    uint state = cell(in, x, y);

    // count the living neighbours
    uint living_neighbours = (
        cell(in, x - 1, y - 1) + cell(in, x, y - 1) + cell(in, x + 1, y - 1) +
        cell(in, x - 1, y)                          + cell(in, x + 1, y) +
        cell(in, x - 1, y + 1) + cell(in, x, y + 1) + cell(in, x + 1, y + 1)
    );

    if (state != 0)
    {
        // only 2 and 3 is good
        // otherwise the cell dies from under-/overpopulation
        if (living_neighbours < 2 || living_neighbours > 3)
            state = 0;
    }
    else
    {
        // with 3 living neighbours reproduction happens :-)
        if (living_neighbours == 3)
            state = 1;
    }

    // return the result
    write_imageui(out, (int2)(x, y), (uint4)(0, 0, 0, state));
}
