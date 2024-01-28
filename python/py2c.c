/*** Includes *********************************************************/
#include <Python.h>

#include "matrix.h"
#include "common.h"

/*** functions ********************************************************/
ret_codes_t
convert_datalist_from_python(
        PyObject * py_data_list,
        DatapointList_t data,
        int dim_count,
        int datapoints_count)
{
    ret_codes_t ret_code = SUCCESS;
    datapoint_t cur_datapoint = NULL;
    int i = 0, j;

    for (i = 0; i < datapoints_count; ++i) {
        allocate_datapoint(&cur_datapoint, dim_count);
        if (NULL == cur_datapoint) {
            ret_code = ERROR;
            goto l_exit;
        }

        for (j = 0; j < dim_count; ++j) {
            cur_datapoint[j] = PyFloat_AsDouble(
                    PyList_GetItem(
                        PyList_GetItem(py_data_list, i),
                        j));
        }

        ret_code = append_to_list(data, cur_datapoint);
        /* Free old cur_datapoint */
        if (ret_code) goto l_exit;
    }

l_exit:
    return ret_code;
}

ret_codes_t
convert_centroids_from_python(
        PyObject * py_centroids,
        datapoint_t * centroids,
        int dim_count,
        int K)
{
    ret_codes_t ret_code = SUCCESS;
    int i = 0, j;

    for (i = 0; i < K; ++i) {
        centroids[i] = (datapoint_t)PyMem_RawMalloc(sizeof(**centroids) * dim_count);
        if (centroids[i] == NULL){
            ret_code = ERROR;
            goto l_exit;
        }

        for (j = 0; j < dim_count; ++j) {
            centroids[i][j]= PyFloat_AsDouble(
                    PyList_GetItem(
                        PyList_GetItem(py_centroids, i),
                        j));
        }
    }

l_exit:
    return ret_code;
}

ret_codes_t convert_matrix_from_python(PyObject * matrix_obj, double *** output_matrix, int N)
{
    ret_codes_t return_code = SUCCESS;
    double ** matrix = NULL;

    int i, j;

    return_code = allocate_matrix(&matrix, N);
    if (return_code) goto l_exit;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            matrix[i][j]= PyFloat_AsDouble(
                    PyList_GetItem(
                        PyList_GetItem(matrix_obj, i),
                        j));
        }
    }

    *output_matrix = matrix;

l_exit:
    return return_code;
}
