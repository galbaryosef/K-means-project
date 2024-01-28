/*** Includes *********************************************************/
#include <stdlib.h>
#include "common.h"

/*** functions ********************************************************/
ret_codes_t allocate_datapoint(datapoint_t * cur_datapoint, int dim_count)
{
    ret_codes_t ret_code = SUCCESS;

    if (NULL == cur_datapoint) {
        ret_code = ERROR;
        goto l_exit;
    }

    *cur_datapoint = (datapoint_t)malloc(sizeof(**cur_datapoint) * (int)dim_count);

    if (NULL == *cur_datapoint) {
        ret_code = ERROR;
        goto l_exit;
    }

l_exit:
    return ret_code;
}

void
sub_datapoints(
        datapoint_t dp0,
        datapoint_t dp1,
        datapoint_t output,
        int dim_count)
{
    int i = 0;
    for (; i < dim_count; ++i) {
        output[i] = dp0[i] - dp1[i];
    }
}
