char cell(__global char const * field, int x, int y, int width, int height)
{
    if (x < 0 || y < 0 || x >= width || y >= height)
        return 0;

    return field[y * width + x];
}

__kernel void GameOfLife(__global char const * restrict in, __global char * restrict out, int width, int height)
{
    int tid = get_global_id(0);
    int x = tid % width;
    int y = tid / width;

    char state = in[tid]; // 0 - dead, 1 - alive

    // count the living neighbours
    int living_neighbours = (
        cell(in, x - 1, y - 1, width, height) + cell(in, x, y - 1, width, height) + cell(in, x + 1, y - 1, width, height) +
        cell(in, x - 1, y, width, height)                                         + cell(in, x + 1, y, width, height) +
        cell(in, x - 1, y + 1, width, height) + cell(in, x, y + 1, width, height) + cell(in, x + 1, y + 1, width, height)
    );

    if (state == 1)
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
    out[tid] = state;
}
