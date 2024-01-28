#ifndef MATRIX_H
#define MATRIX_H
/*** Includes *********************************************************/
#include <stdbool.h>
#include "common.h"

/*** Prototypes *******************************************************/
ret_codes_t allocate_matrix(double *** matrix, int N);
ret_codes_t free_matrix(double *** matrix, int N);
ret_codes_t copy_matrix(double *** output_matrix, double **src_matrix, int N);
void print_matrix(double ** matrix, int N);
void multiply_2_matrices(double ** dst_matrix, double ** src_matrix, int N);
double offMatrix(double ** matrix, int N);
void zero_matrix(double ** matrix, int N);
void set_value_in_diagonal(double **matrix, int N, double value);

#endif  /* MATRIX_H */
