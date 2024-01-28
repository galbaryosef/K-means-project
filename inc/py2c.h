#ifndef PY_2_C_H
#define PY_2_C_H
/*** Includes *********************************************************/
#include "common.h"

/*** Prototypes *******************************************************/
ret_codes_t convert_datalist_from_python(PyObject *, DatapointList_t, int, int);
ret_codes_t convert_centroids_from_python(PyObject *, datapoint_t *, int, int);
ret_codes_t convert_matrix_from_python(PyObject *, double ***, int);
#endif  /* PY_2_C_H */
