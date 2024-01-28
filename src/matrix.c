/*** Includes *********************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#include "common.h"

/*** Functions ********************************************************/
ret_codes_t allocate_matrix(double *** matrix, int N)
{
    ret_codes_t return_code = SUCCESS;
    double ** local_matrix = NULL;
    int i;

    local_matrix = (double **)malloc(N * sizeof(*local_matrix));
    if (NULL == local_matrix) {
        return_code = ERROR;
        goto l_exit;
    }

    for (i = 0; i < N; ++i) {
        local_matrix[i] = (double *)malloc(N * sizeof(**local_matrix));
        if (NULL == local_matrix[i]) {
            return_code = ERROR;
            goto l_exit;
        }
    }

    *matrix = local_matrix;

l_exit:
    return return_code;
}

ret_codes_t free_matrix(double *** output_matrix, int N)
{
    ret_codes_t return_code = SUCCESS;
    int i = 0;

    if (NULL == output_matrix) {
        return_code = ERROR;
        goto l_exit;
    }

    if (NULL == (*output_matrix)) goto l_exit;

    for (i = 0; i < N; ++i) {
        free((*output_matrix)[i]);
    }

    free(*output_matrix);
    *output_matrix = NULL;

l_exit:
    return return_code;
}

void zero_matrix(double ** matrix, int N)
{
    int i = 0, j = 0;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void set_value_in_diagonal(double **matrix, int N, double value)
{
    int i = 0;

    for (i = 0; i < N; ++i) {
        matrix[i][i] = value;
    }
}

ret_codes_t copy_matrix(double *** output_matrix, double **src_matrix, int N)
{
    ret_codes_t return_code = SUCCESS;
    int i = 0;
    double ** local_matrix = NULL;

    local_matrix = (double **)malloc(N * sizeof(*local_matrix));
    if (NULL == local_matrix) {
        return_code = ERROR;
        goto l_exit;
    }

    for (i = 0; i < N; ++i) {
        local_matrix[i] = (double *)malloc(N * sizeof(**local_matrix));
        memcpy((void *)(local_matrix[i]),
                (void *)(src_matrix[i]),
                N * sizeof(**local_matrix));
    }

    *output_matrix = local_matrix;

l_exit:
    return return_code;
}

void print_matrix(double ** matrix, int N)
{
    int i = 0;
    int j = 0;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (j == (N - 1)) {
                printf("%.4f\n", matrix[i][j]);
            } else {
                printf("%.4f,", matrix[i][j]);
            }
        }
    }
}

void multiply_2_matrices(double ** dst_matrix, double ** src_matrix, int N) 
{
    int i = 0, j = 0, k = 0;
    double ** output_matrix = NULL;

    allocate_matrix(&output_matrix, N);
    zero_matrix(output_matrix, N);

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            output_matrix[i][j] = 0;

            for (k = 0; k < N; ++k) {
                output_matrix[i][j] += dst_matrix[i][k] * src_matrix[k][j];
            }
        }
    }

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            dst_matrix[i][j] = output_matrix[i][j];
        }
    }

    free_matrix(&output_matrix, N);
}

double offMatrix(double ** matrix, int N)
{
    double sum = 0;
    int i, j;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (i != j) {
                sum += pow(matrix[i][j], 2);
            }
        }
    }

    return sum;
}

