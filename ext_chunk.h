#ifndef __CHUNK
#define __CHUNK

// Global chunk class
#include "ext_profiler.h"

typedef struct
{
    int offload;
} CloverChunk;

#pragma omp declare target
extern CloverChunk _chunk;
#pragma omp end declare target

#endif
