#ifndef COMMON_H
#define COMMON_H

/*** Enums ************************************************************/
typedef enum {
    SUCCESS = 0,
    ERROR
} ret_codes_t;

/*** Includes *********************************************************/
#include "datapoint_list.h"

/*** Defines (*********************************************************/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define G_JACOBI_MAX_ITER (100)
#define G_JACOBI_EPSILON (0.00001)

#define G_KMEANS_MAX_ITER (300)
#define G_KMEANS_EPSILON (0.0)

#endif  /* COMMON_H */
