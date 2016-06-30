#ifndef __CHUNK
#define __CHUNK

// Global chunk class
#include "ext_profiler.h"

typedef struct
{
    int xmax;
    int ymax;
    int offload;
} CloverChunk;

extern CloverChunk _chunk;

#endif
