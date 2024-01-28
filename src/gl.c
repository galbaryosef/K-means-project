/*** Includes *********************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "gl.h"
#include "wam.h"
#include "ddg.h"
#include "matrix.h"
#include "common.h"

/*** Functions ********************************************************/
ret_codes_t gl(double *** output_laplacian, DatapointList_t data)
{
    ret_codes_t return_code = SUCCESS;

    double ** laplacian = NULL;
    double ** weighted = NULL;
    double ** diagonal = NULL;

    int i = 0, j = 0;
    
    laplacian = (double **)malloc(data->length * sizeof(*laplacian));
    if (NULL == laplacian) {
        return_code = ERROR;
        goto l_exit;
    }

    for (i = 0; i < data->length; ++i) {
        laplacian[i] = (double *)malloc(data->length * sizeof(**laplacian));
        if (NULL == laplacian[i]) {
            return_code = ERROR;
            goto l_exit;
        }
    }

    return_code = wam(&weighted, data);
    if (return_code) goto l_exit;

    return_code = ddg(&diagonal, data);
    if (return_code) goto l_exit;

    for (i = 0; i < data->length; ++i){
        for (j = 0; j < data->length; ++j){
            laplacian[i][j] = diagonal[i][j] - weighted[i][j];
        }
    }

    *output_laplacian = laplacian;

l_exit:
    free_matrix(&weighted, data->length);
    free_matrix(&diagonal, data->length);
    return return_code;
}
