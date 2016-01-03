/*Crown Copyright 2012 AWE.
 *
 * This file is part of CloverLeaf.
 *
 * CloverLeaf is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * CloverLeaf is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * CloverLeaf. If not, see http://www.gnu.org/licenses/. */

/**
 *  @brief C ideal gas kernel.
 *  @author Wayne Gaudin
 *  @details Calculates the pressure and sound speed for the mesh chunk using
 *  the ideal gas equation of state, with a fixed gamma of 1.4.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ftocmacros.h"
#include "ext_chunk.h"

void ideal_gas_kernel_c_(int *xmin,int *xmax,int *ymin,int *ymax,
        double *density,
        double *energy,
        double *pressure,
        double *soundspeed,
        int* offload)
{
    int x_min=*xmin;
    int x_max=*xmax;
    int y_min=*ymin;
    int y_max=*ymax;
    int ideal_offload = *offload && _chunk.offload;

    printf("in %s offload: %d\n", __func__, ideal_offload);

#pragma omp target teams distribute if(ideal_offload)
#pragma omp parallel for
    for (int k = y_min; k <= y_max; k++) 
    {
//#pragma ivdep
        for (int j = x_min; j <= x_max; j++) 
        { 
            double v = 1.0/density[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)];
            pressure[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)] = (1.4-1.0) *
                density[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)] *
                energy[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)];
            double pressurebyenergy = (1.4-1.0) *
                density[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)];
            double pressurebyvolume = -density[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)] *
                pressure[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)];
            double sound_speed_squared = v*v*
                (pressure[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)] * 
                 pressurebyenergy-pressurebyvolume);
            soundspeed[FTNREF2D(j  ,k  ,x_max+4,x_min-2,y_min-2)] = 
                sqrt(sound_speed_squared);
        }
    }

    printf("out %s offload: %d\n", __func__, ideal_offload);
}
