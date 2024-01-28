/*** Includes *********************************************************/
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#include "common.h"
#include "matrix.h"

/*** Prototypes *******************************************************/
double calculate_exp(DatapointNode_t node_i, DatapointNode_t node_j, int dim_count);
double square_and_sum_diff_vector(datapoint_t diff, int dim_count);

/*** Functions ********************************************************/
ret_codes_t wam(double *** output_matrix, DatapointList_t data)
{
    ret_codes_t return_code = SUCCESS;
    DatapointNode_t node_i = NULL;
    DatapointNode_t node_j = NULL;
    double my_exp;

    double ** matrix = NULL;
    int i = 0, j = 0;

    matrix = (double **)malloc(data->length * sizeof(*matrix));
    if (NULL == matrix) {
        return_code = ERROR;
        goto l_exit;
    }

    for (i = 0; i < data->length; ++i) {
        matrix[i] = (double *)malloc(data->length * sizeof(**matrix));
        if (NULL == matrix[i]) {
            return_code = ERROR;
            goto l_exit;
        }
    }

    node_i = data->head;

    for (i = 0; i < data->length; ++i) {
        node_j = data->head;

        for (j = 0; j < data->length; ++j) {
            if (i == j) {
                my_exp = 0;
            } else {
                my_exp = calculate_exp(node_i, node_j, data->dim_count);
            }

            matrix[i][j] = my_exp;
            matrix[j][i] = my_exp;

            node_j = node_j->next;
        }

        node_i = node_i->next;
    }

    *output_matrix = matrix;

l_exit:
    return return_code;
}

double calculate_exp(DatapointNode_t node_i, DatapointNode_t node_j, int dim_count)
{
    datapoint_t diff;
    double my_exp;
    double sum;

    allocate_datapoint(&diff, dim_count);
    sub_datapoints(node_i->value, node_j->value, diff, dim_count);
    sum = square_and_sum_diff_vector(diff, dim_count);
    my_exp = exp(sum / -2);

    free(diff);

    return my_exp;
}

double square_and_sum_diff_vector(datapoint_t diff, int dim_count)
{
    int i = 0;
    double sum = 0;

    for (i = 0; i < dim_count; ++i) {
        sum += diff[i] * diff[i];
    }

    return sum;
}
