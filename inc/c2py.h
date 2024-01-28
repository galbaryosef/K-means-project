#ifndef C_2_PY_H
#define C_2_PY_H
/*** Includes *********************************************************/
#include "common.h"

/*** Prototypes *******************************************************/
ret_codes_t convert_matrix_to_py(double ** matrix, PyObject ** matrix_obj, int N);
ret_codes_t convert_centroids_to_python(datapoint_t *, PyObject **, int, int);
ret_codes_t convert_vector_to_py(double * vector, PyObject ** vector_obj, int N);

#endif  /* C_2_PY_H */
