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
 *  @brief OpenMP 4.5 target data offloading.
 *  @author Justin Salmon
 *  @details Offloads all required data to the target device.
 */

#include <stdio.h>
#include "ext_chunk.h"

void map_to_device_c_(int *xmin, int *xmax, int *ymin, int *ymax,
                      double *density0,
                      double *density1,
                      double *energy0,
                      double *energy1,
                      double *pressure,
                      double *viscosity,
                      double *soundspeed,
                      double *xvel0,
                      double *xvel1,
                      double *yvel0,
                      double *yvel1,
                      double *vol_flux_x,
                      double *vol_flux_y,
                      double *mass_flux_x,
                      double *mass_flux_y,
                      double *work_array1,
                      double *work_array2,
                      double *work_array3,
                      double *work_array4,
                      double *work_array5,
                      double *work_array6,
                      double *work_array7,
                      double *cellx,
                      double *celly,
                      double *vertexx,
                      double *vertexy,
                      double *celldx,
                      double *celldy,
                      double *vertexdx,
                      double *vertexdy,
                      double *xarea,
                      double *yarea,
                      double *volume,
                      int *offload) {
    int x_min = *xmin;
    int x_max = *xmax;
    int y_min = *ymin;
    int y_max = *ymax;
    int ideal_offload = *offload && _chunk.offload;
    int max = x_max * y_max;

 #pragma omp target enter data if(ideal_offload) \
    map(to: density0[0:(x_max+4)*(y_max+4)], density1[0:(x_max+4)*(y_max+4)], energy0[0:(x_max+4)*(y_max+4)], energy1[0:(x_max+4)*(y_max+4)]) \
    map(to: pressure[0:(x_max+4)*(y_max+4)], viscosity[0:(x_max+4)*(y_max+4)], soundspeed[0:(x_max+4)*(y_max+4)], xvel0[0:(x_max+5)*(y_max+5)]) \
    map(to: xvel1[0:(x_max+5)*(y_max+5)], yvel0[0:(x_max+5)*(y_max+5)], yvel1[0:(x_max+5)*(y_max+5)], vol_flux_x[0:(x_max+5)*(y_max+4)]) \
    map(to: mass_flux_x[0:(x_max+5)*(y_max+4)], vol_flux_y[0:(x_max+4)*(y_max+5)], mass_flux_y[0:(x_max+4)*(y_max+5)]) \
    map(to: work_array1[0:(x_max+5)*(y_max+5)], work_array2[0:(x_max+5)*(y_max+5)], work_array3[0:(x_max+5)*(y_max+5)]) \
    map(to: work_array4[0:(x_max+5)*(y_max+5)], work_array5[0:(x_max+5)*(y_max+5)], work_array6[0:(x_max+5)*(y_max+5)]) \
    map(to: work_array7[0:(x_max+5)*(y_max+5)], cellx[0:x_max+4], celly[0:y_max+4], vertexx[0:x_max+5]) \
    map(to: vertexy[0:y_max+5], celldx[0:x_max+4], celldy[0:y_max+4], vertexdx[0:x_max+5]) \
    map(to: vertexdy[0:y_max+5], xarea[0:(x_max+5)*(y_max+4)], yarea[0:(x_max+4)*(y_max+5)], volume[0:(x_max+4)*(y_max+4)])
}

void update_from_device_c_(int *xmin, int *xmax, int *ymin, int *ymax,
                      double *density0,
                      double *energy0,
                      double *pressure,
                      double *xvel0,
                      double *yvel0)
{
    int x_min = *xmin;
    int x_max = *xmax;
    int y_min = *ymin;
    int y_max = *ymax;

#pragma omp target update if(_chunk.offload) \
    from(density0[0:(x_max+4)*(y_max+4)], energy0[0:(x_max+4)*(y_max+4)], pressure[0:(x_max+4)*(y_max+4)]) \
    from(xvel0[0:(x_max+5)*(y_max+5)], yvel0[0:(x_max+5)*(y_max+5)])
}

