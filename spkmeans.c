/*** Defines (*********************************************************/
#define _GNU_SOURCE

/*** Includes *********************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "common.h"
#include "matrix.h"
#include "spkmeans.h"
#include "wam.h"
#include "ddg.h"
#include "gl.h"
#include "jacobi.h"
#include "spk.h"

/*** functions ********************************************************/
ret_codes_t parse_args(int argc, char *argv[], char ** goal, char ** file_name)
{
    ret_codes_t ret_code = SUCCESS;
    (void) argc;

    if ((NULL == goal) ||
        (NULL == file_name)) {
        ret_code = ERROR;
        goto l_exit;
    }

    *goal = argv[1];
    *file_name = argv[2];

l_exit:
    return ret_code;
}

void count_dims(const char * line, size_t len, int * dim_count)
{
    size_t i = 0;

    for (; i < len; ++i) {
        if (',' == line[i]) {
            ++(*dim_count);
        }
    }

    ++(*dim_count);
}

ret_codes_t parse_matrix_from_input(char * file_name, double *** matrix, int *N)
{
    ret_codes_t return_code = SUCCESS;

    double ** local_matrix = NULL;
    FILE * fp = NULL;

    char * line_buf = NULL;
    char * cur_line_part = NULL;
    char end_line;
    char * end;

    int i = 0, j = 0;

    fp = fopen(file_name, "r");
    if (NULL == fp) {
        return_code = ERROR;
        goto l_exit;
    }

    line_buf = (char *)malloc(sizeof(*line_buf) * 4096);
    if (NULL == line_buf) {
        return_code = ERROR;
        goto l_exit;
    }

    while (2 == fscanf(fp, "%s%c", line_buf, &end_line)) {
        if ('\n' != end_line) break;
        if ('\0' == (*line_buf)) break;

        if (NULL == local_matrix) {
            *N = 0;
            count_dims(line_buf, strnlen(line_buf, 4096), N);
            return_code = allocate_matrix(&local_matrix, *N);
            if (return_code) goto l_exit;
        }

        cur_line_part = line_buf;

        for (j = 0; j < (*N); ++j) {
            local_matrix[i][j] = strtod(cur_line_part, &end);
            if ((end == cur_line_part) || ((*end != ',') && (*end != '\0'))) {
                return_code = ERROR;
                goto l_exit;
            }

            cur_line_part = end + 1;
        }

        if (*end != '\0') {
            return_code = ERROR;
            goto l_exit;
        }

        ++i;
    }

    *matrix = local_matrix;

l_exit:
    if (NULL != line_buf) free(line_buf);
    if (NULL != fp) fclose(fp);

    return return_code;
}

ret_codes_t parse_data_from_input(char * file_name, DatapointList_t data, int *dim_count)
{
    ret_codes_t ret_code = 0;

    char * cur_line_part = NULL;
    char * line_buf = NULL;
    char * end;
    FILE * fp = NULL;

    size_t line_buf_len = 0;
    char end_line;

    int i = 0;
    datapoint_t cur_datapoint = NULL;

    fp = fopen(file_name, "r");
    if (NULL == fp) {
        ret_code = ERROR;
        goto l_exit;
    }

    *dim_count = 0;

    line_buf = (char *)malloc(sizeof(*line_buf) * 4096);
    if (NULL == line_buf) {
        ret_code = ERROR;
        goto l_exit;
    }

    while (2 == fscanf(fp, "%s%c", line_buf, &end_line)) {
        if ('\n' != end_line) break;
        if ('\0' == (*line_buf)) break;

        line_buf_len = strnlen(line_buf, 4096);

        if (0 == (*dim_count)) {
            count_dims(line_buf, line_buf_len, dim_count);
            data->dim_count = *dim_count;
        }

        cur_datapoint = (datapoint_t)malloc(sizeof(*cur_datapoint) * (*dim_count));
        if (cur_datapoint == NULL){
            ret_code = ERROR;
            goto l_exit;
        }

        cur_line_part = line_buf;

        for (i = 0; i < (*dim_count); ++i) {
            cur_datapoint[i] = strtod(cur_line_part, &end);
            if ((end == cur_line_part) || ((*end != ',') && (*end != '\0'))) {
                ret_code = ERROR;
                goto l_exit;
            }

            cur_line_part = end + 1;
        }

        if (*end != '\0') {
            ret_code = ERROR;
            goto l_exit;
        }

        line_buf_len = 0;

        ret_code = append_to_list(data, cur_datapoint);
        if (ret_code) {
            goto l_exit;
        }
    }

l_exit:
    if (NULL != line_buf) free(line_buf);
    if (NULL != fp) fclose(fp);

    return ret_code;
}

int main(int argc, char *argv[])
{
    ret_codes_t ret_code = SUCCESS;

    DatapointList_t data = NULL;

    char *goal;
    char *file_name;

    int dim_count;
    int N;  /* Size of any of the dimensions of matrix */

    double * eigenvalues;
    double ** eigenvectors_matrix;
    double ** matrix = NULL;

    ret_code = init_datapoint_list(&data);
    if (ret_code) goto l_exit;

    ret_code = parse_args(argc, argv, &goal, &file_name);
    if (ret_code) goto l_exit;

    if (strcmp(goal, "jacobi")) {
        ret_code = parse_data_from_input(file_name, data, &dim_count);
        if (ret_code) goto l_exit;
    } else {
        ret_code = parse_matrix_from_input(file_name, &matrix, &N);
        if (ret_code) goto l_exit;
    }

    if (!strcmp(goal, "wam")) {
        ret_code = wam(&matrix, data);
        if (ret_code) goto l_exit;
        print_matrix(matrix, data->length);
        free_matrix(&matrix, data->length);

    } else if (!strcmp(goal, "ddg")) {
        ret_code = ddg(&matrix, data);
        if (ret_code) goto l_exit;
        print_matrix(matrix, data->length);
        free_matrix(&matrix, data->length);

    } else if (!strcmp(goal, "gl")) {
        ret_code = gl(&matrix, data);
        if (ret_code) goto l_exit;
        print_matrix(matrix, data->length);
        free_matrix(&matrix, data->length);

    } else if (!strcmp(goal, "jacobi")) {
        ret_code = jacobi(matrix, N, &eigenvalues, &eigenvectors_matrix);
        if (ret_code) goto l_exit;
        print_vector(eigenvalues, N);
        print_matrix(eigenvectors_matrix, N);
        free_matrix(&matrix, N);

    } else {
        ret_code = ERROR;
    }

l_exit:
    destroy_list(&data);

    if (ret_code) printf("An Error Has Occurred\n");

    return (int) ret_code;
}
