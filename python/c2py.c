/*** Includes *********************************************************/
#include <Python.h>

#include "c2py.h"

/*** functions ********************************************************/
ret_codes_t
convert_centroids_to_python(
        datapoint_t * centroids,
        PyObject ** py_list,
        int datapoint_dim_count,
        int K)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * cur_py_centroid;
    PyObject * cur_python_value;

    int i = 0, j;

    *py_list = PyList_New(K);

    for (i = 0; i < K; ++i) {
        cur_py_centroid = PyList_New(datapoint_dim_count);

        for (j = 0; j < datapoint_dim_count; ++j) {
            cur_python_value = Py_BuildValue("d", centroids[i][j]);
            PyList_SetItem(cur_py_centroid, j, cur_python_value);
        }

        PyList_SetItem(*py_list, i, cur_py_centroid);
    }

    return ret_code;
}

ret_codes_t convert_matrix_to_py(double ** matrix, PyObject ** matrix_obj, int N)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * matrix_line;
    int i = 0, j =0;
    PyObject * cur_value;

    *matrix_obj = PyList_New(N);

    for (i = 0; i < N; ++i) {
        matrix_line = PyList_New(N);

        for (j = 0; j < N; ++j) {
            cur_value = Py_BuildValue("d", matrix[i][j]);
            PyList_SetItem(matrix_line, j, cur_value);
        }

        PyList_SetItem(*matrix_obj, i, matrix_line);
    }
    return ret_code;
}

ret_codes_t convert_vector_to_py(double * vector, PyObject ** vector_obj, int N)
{
    ret_codes_t ret_code = SUCCESS;
    int i = 0;
    PyObject * cur_value;

    *vector_obj = PyList_New(N);

    for (i = 0; i < N; ++i) {
        cur_value = Py_BuildValue("d", vector[i]);
        PyList_SetItem(*vector_obj, i, cur_value);
    }
    return ret_code;
}
