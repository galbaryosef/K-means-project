/*** Defines **********************************************************/
#define PY_SSIZE_T_CLEAN

/*** Includes *********************************************************/
#include <Python.h>

#include "py2c.h"
#include "c2py.h"
#include "matrix.h"

#include "wam.h"
#include "ddg.h"
#include "gl.h"
#include "jacobi.h"
#include "spk.h"

/*** functions ********************************************************/
static PyObject * wam_wrapper_for_py(PyObject *self, PyObject *args)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * data_obj;
    PyObject * matrix_obj = NULL;
    DatapointList_t data_list = NULL;

    double ** matrix;

    int datapoints_count, datapoint_dim_count;

    if (!PyArg_ParseTuple(
                args,
                "Oii",
                &data_obj,
                (int *)&datapoints_count,
                (int *)&datapoint_dim_count))
        goto l_exit;

    ret_code = init_datapoint_list(&data_list);
    if (ret_code) goto l_exit;

    data_list->dim_count = datapoint_dim_count;
    ret_code = convert_datalist_from_python(
            data_obj,
            data_list,
            datapoint_dim_count,
            datapoints_count);
    if (ret_code) goto l_exit;

    ret_code = wam(&matrix, data_list);
    if (ret_code) goto l_exit;
    convert_matrix_to_py(matrix, &matrix_obj, data_list->length);

l_exit:
    free_matrix(&matrix, data_list->length);
    destroy_list(&data_list);
    return matrix_obj;
}

static PyObject * ddg_wrapper_for_py(PyObject *self, PyObject *args)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * data_obj = NULL;
    PyObject * matrix_obj = NULL;
    DatapointList_t data_list = NULL;

    double ** matrix;

    int datapoints_count, datapoint_dim_count;

    if (!PyArg_ParseTuple(
                args,
                "Oii",
                &data_obj,
                (int *)&datapoints_count,
                (int *)&datapoint_dim_count))
        goto l_exit;

    ret_code = init_datapoint_list(&data_list);
    if (ret_code) goto l_exit;

    data_list->dim_count = datapoint_dim_count;
    ret_code = convert_datalist_from_python(
            data_obj,
            data_list,
            datapoint_dim_count,
            datapoints_count);
    if (ret_code) goto l_exit;

    ret_code = ddg(&matrix, data_list);
    if (ret_code) goto l_exit;
    convert_matrix_to_py(matrix, &matrix_obj, data_list->length);

l_exit:
    free_matrix(&matrix, data_list->length);
    destroy_list(&data_list);
    return matrix_obj;
}

static PyObject * gl_wrapper_for_py(PyObject *self, PyObject *args)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * data_obj = NULL;
    PyObject * matrix_obj = NULL;
    DatapointList_t data_list = NULL;

    double ** matrix;

    int datapoints_count, datapoint_dim_count;

    if (!PyArg_ParseTuple(
                args,
                "Oii",
                &data_obj,
                (int *)&datapoints_count,
                (int *)&datapoint_dim_count))
        goto l_exit;

    ret_code = init_datapoint_list(&data_list);
    if (ret_code) goto l_exit;

    data_list->dim_count = datapoint_dim_count;
    ret_code = convert_datalist_from_python(
            data_obj,
            data_list,
            datapoint_dim_count,
            datapoints_count);
    if (ret_code) goto l_exit;

    ret_code = gl(&matrix, data_list);
    if (ret_code) goto l_exit;
    convert_matrix_to_py(matrix, &matrix_obj, data_list->length);

l_exit:
    free_matrix(&matrix, data_list->length);
    destroy_list(&data_list);
    return matrix_obj;
}

static PyObject * jacobi_wrapper_for_py(PyObject *self, PyObject *args)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * matrix_obj = NULL;
    PyObject * eigenvalues_obj = NULL;
    PyObject * eigenvectors_obj = NULL;

    double ** matrix;
    double ** eigenvectors_matrix;
    double * eigenvalues;

    int N;

    if (!PyArg_ParseTuple(
                args,
                "Oi",
                &matrix_obj,
                (int *)&N))
        goto l_exit;

    ret_code = convert_matrix_from_python(matrix_obj, &matrix, N);
    if (ret_code) goto l_exit;

    ret_code = jacobi(matrix, N, &eigenvalues, &eigenvectors_matrix);
    if (ret_code) goto l_exit;

    convert_matrix_to_py(eigenvectors_matrix, &eigenvectors_obj, N);
    convert_vector_to_py(eigenvalues, &eigenvalues_obj, N);
    //tuplize(&output_obj, eigenvalues_obj, eigenvectors_obj);

l_exit:
    free_matrix(&matrix, N);
    return Py_BuildValue("(OO)", eigenvalues_obj, eigenvectors_obj);
}

static PyObject * spk_wrapper_for_py(PyObject *self, PyObject *args)
{
    ret_codes_t ret_code = SUCCESS;
    PyObject * centroids_obj;
    PyObject * data_obj;
    PyObject * result_py_centroids = NULL;
    DatapointList_t data_list = NULL;
    datapoint_t * initial_centroids = NULL;

    int K;
    int datapoints_count, datapoint_dim_count;

    (void) self;

    if (!PyArg_ParseTuple(
                args,
                "OOiii",
                &centroids_obj,
                &data_obj,
                &K,
                (int *)&datapoints_count,
                (int *)&datapoint_dim_count))
        return NULL;

    ret_code = init_datapoint_list(&data_list);
    if (ret_code) goto l_exit;

    data_list->dim_count = datapoint_dim_count;
    ret_code = convert_datalist_from_python(
            data_obj,
            data_list,
            datapoint_dim_count,
            datapoints_count);
    if (ret_code) goto l_exit;

    initial_centroids = (datapoint_t *)PyMem_RawMalloc(sizeof(*initial_centroids) * K);
    if (NULL == initial_centroids) {
        printf("An Error Has Occurred\n");
        goto l_exit;
    }
    ret_code = convert_centroids_from_python(
            centroids_obj,
            initial_centroids,
            datapoint_dim_count,
            K);
    if (ret_code) goto l_exit;

    ret_code = kmeans(data_list, initial_centroids, K);
    if (ret_code) goto l_exit;

    ret_code = convert_centroids_to_python(
            initial_centroids, /* Now the initial_centroids hold the new centroids */
            &result_py_centroids,
            datapoint_dim_count,
            K);
    if (ret_code) goto l_exit;

l_exit:
    clean_centroids(&initial_centroids, K);
    destroy_list(&data_list);
    return result_py_centroids;
}

static PyMethodDef kmeans_methods[] = {
    {"wam",
      (PyCFunction) wam_wrapper_for_py,
      METH_VARARGS,
      PyDoc_STR("Kmeans wam implementation")},
    {"ddg",
      (PyCFunction) ddg_wrapper_for_py,
      METH_VARARGS,
      PyDoc_STR("Kmeans ddg implementation")},
    {"gl",
      (PyCFunction) gl_wrapper_for_py,
      METH_VARARGS,
      PyDoc_STR("Kmeans gl implementation")},
    {"jacobi",
      (PyCFunction) jacobi_wrapper_for_py,
      METH_VARARGS,
      PyDoc_STR("Kmeans jacobi implementation")},
    {"spk",
      (PyCFunction) spk_wrapper_for_py,
      METH_VARARGS,
      PyDoc_STR("Kmeans implementation")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef kmeans_module = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    NULL,
    -1, 
    kmeans_methods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *module;

    module = PyModule_Create(&kmeans_module);
    if (!module) {
        return NULL;
    }

    return module;
}
