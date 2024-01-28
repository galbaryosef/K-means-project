#ifndef DATAPOINT_H
#define DATAPOINT_H
/*** Includes *********************************************************/
#include "common.h"

/*** types ************************************************************/
typedef double * datapoint_t;

/*** Prototypes *******************************************************/
ret_codes_t allocate_datapoint(datapoint_t * cur_datapoint, int dim_count);
void sub_datapoints(datapoint_t , datapoint_t , datapoint_t , int);
#endif  /* DATAPOINT_H */
