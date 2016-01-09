#include "ext_chunk.h"

#pragma omp declare target
CloverChunk _chunk;
#pragma omp end declare target

void ext_init_(
        int* xmax,
        int* ymax,
        int* offload)
{
#ifdef OFFLOAD
    _chunk.offload = 1;
#else
    _chunk.offload = 0;
#endif
 
    _chunk.xmax = *xmax;
    _chunk.ymax = *ymax;
    *offload = _chunk.offload;

#pragma omp target update if(_chunk.offload) to(_chunk)
}

void ext_finalise_()
{
    PRINT_PROFILING_RESULTS;
}
