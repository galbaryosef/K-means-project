/*** Includes *********************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "ddg.h"
#include "wam.h"
#include "matrix.h"
#include "common.h"

/*** Functions ********************************************************/
ret_codes_t ddg(double *** output_diagonal, DatapointList_t data)
{
    ret_codes_t return_code = SUCCESS;
    
    double ** weighted = NULL;
    double ** diagonal = NULL;

    int i = 0;
    int j = 0;

    return_code = allocate_matrix(&diagonal, data->length);
    if (return_code) goto l_exit;
    zero_matrix(diagonal, data->length);

    return_code = wam(&weighted, data);
    if (return_code) goto l_exit;

    for (i = 0; i < data->length; ++i){
        for (j = 0; j < data->length; ++j){
            diagonal[i][i] += weighted[i][j];
        }
    }

    *output_diagonal = diagonal;

l_exit:
    free_matrix(&weighted, data->length);
    return return_code;
}
