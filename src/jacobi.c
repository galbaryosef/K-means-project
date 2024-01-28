/*** Includes *********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "matrix.h"

/*** Prototypes *******************************************************/
ret_codes_t rotate_matrix(double **, double ***, double *, double *, int, int *, int *);
double sign(double num);
ret_codes_t transform_matrix(double **, double, double, double ***, int, int, int);
bool converged(double ** matrix0, double ** matrix1, int N);
void find_max_val(double ** matrix, int N, int * i_pivot, int * j_pivot);
void align_neg_zero(double * eigenvalues, double ** eigenvectors_matrix, int N);
ret_codes_t get_diagonal_from_matrix(double ** eigenvalues, double ** new_matrix, int N);

/*** Functions ********************************************************/
ret_codes_t
jacobi(
    double ** matrix,
    int N,
    double ** output_eigenvalues,
    double *** output_eigenvectors_matrix)
{
    ret_codes_t return_code = SUCCESS;
    double ** cur_matrix = matrix;  /* A matrix */
    double ** P_matrix = NULL;
    double ** new_matrix = NULL;  /* A' matrix */
    double ** eigenvectors_matrix = NULL;  /* V matrix*/
    double * eigenvalues; /*eigenvalues vector*/
    double elem_c, elem_s;
    int i = 0, i_pivot, j_pivot;

    for (i = 0; i < G_JACOBI_MAX_ITER; ++i) {
        return_code = rotate_matrix(
            cur_matrix,
            &P_matrix,
            &elem_c,
            &elem_s,
            N,
            &i_pivot,
            &j_pivot);
        if (return_code) goto l_exit;

        if (0 == i) {
            return_code = copy_matrix(&eigenvectors_matrix, P_matrix, N);
            if (return_code) goto l_exit;
        } else {
            multiply_2_matrices(eigenvectors_matrix, P_matrix, N);
        }

        return_code = transform_matrix(
            cur_matrix,
            elem_c,
            elem_s,
            &new_matrix,
            N,
            i_pivot,
            j_pivot);
        if (return_code) goto l_exit;

        if (converged(cur_matrix, new_matrix, N)) {
            free_matrix(&P_matrix, N);
            break;
        }

        if (cur_matrix != matrix) {
            free_matrix(&cur_matrix, N);
        }
        cur_matrix = new_matrix;
    }

    return_code = get_diagonal_from_matrix(&eigenvalues, new_matrix, N);
    if (return_code) goto l_exit;

    align_neg_zero(eigenvalues, eigenvectors_matrix, N);

    *output_eigenvectors_matrix = eigenvectors_matrix;
    *output_eigenvalues = eigenvalues;

l_exit:
    free_matrix(&P_matrix, N);
    return return_code;
}

ret_codes_t
rotate_matrix(
    double ** matrix,
    double *** output_P_matrix,
    double * elem_c,
    double * elem_s,
    int N,
    int * i_pivot,
    int * j_pivot) 
{
    ret_codes_t return_code = SUCCESS;
    double ** P_matrix = NULL;
    int i, j;
    double theta, t;
    
    find_max_val(matrix, N, &i, &j);

    theta = (matrix[j][j] - matrix[i][i]) / (2 * matrix[i][j]);
    t = sign(theta) / (fabs(theta) + sqrt(theta * theta + 1));
    *elem_c = 1 / sqrt(t * t + 1);
    *elem_s = t * (*elem_c);

    *i_pivot = i;
    *j_pivot = j;

    return_code = allocate_matrix(&P_matrix, N);
    if (return_code) goto l_exit;
    zero_matrix(P_matrix, N);
    set_value_in_diagonal(P_matrix, N, 1.0);

    P_matrix[i][i] = *elem_c;
    P_matrix[j][j] = *elem_c;
    P_matrix[i][j] = *elem_s;
    P_matrix[j][i] = -1 * (*elem_s);

    *output_P_matrix = P_matrix;

l_exit:
    return return_code;
}

double sign(double num) 
{
    if (num >= 0.0) {
        return 1.0;
    }
    else return -1.0;
}

ret_codes_t
transform_matrix(
    double ** matrix,
    double elem_c,
    double elem_s,
    double *** output_matrix,
    int N,
    int i,
    int j)
{
    ret_codes_t return_code = SUCCESS;

    double ** new_matrix = NULL;
    int r = 0, m = 0;

    return_code = allocate_matrix(&new_matrix, N);
    if (return_code) goto l_exit;
    zero_matrix(new_matrix, N);

    /*calculate column i and line i*/
    for (r = 0; r < N; ++r) {
        if (r != i && r != j) {
            new_matrix[r][i] = new_matrix[i][r] = elem_c * matrix[r][i] - \
                                                  elem_s * matrix[r][j];
            new_matrix[r][j] = new_matrix[j][r] = elem_c * matrix[r][j] + \
                                                  elem_s * matrix[r][i];
        }
    }

    new_matrix[i][i] = elem_c * elem_c * matrix[i][i] + \
                       elem_s * elem_s * matrix[j][j] - \
                       2 * elem_c * elem_s * matrix[i][j];

    new_matrix[j][j] = elem_s * elem_s * matrix[i][i] + \
                       elem_c * elem_c * matrix[j][j] + \
                       2 * elem_c * elem_s * matrix[i][j];

    new_matrix[i][j] = new_matrix[j][i] = 0;

    /*for the rest of the matrix take the values from matrix*/
    for (r = 0; r < N; ++r) {
        if (r != i && r != j) {
            for (m = 0; m < N; ++m) {
                if (m != i && m != j)
                    new_matrix[r][m] = matrix[r][m];
            }
        }
    }

    *output_matrix = new_matrix;

l_exit:
    return return_code;
}

void find_max_val(double ** matrix, int N, int * i_pivot, int * j_pivot)
{
    double val;
    int iVal = 0;
    int jVal = 1;
    int i, j;

    val = fabs(matrix[0][1]);

    for (i = 0; i < N; ++i) {
        for (j = i + 1; j < N; ++j) {
            if (fabs(matrix[i][j]) > val) {
                val = fabs(matrix[i][j]);
                iVal = i;
                jVal = j;
            }
        }
    }

    *i_pivot = iVal;
    *j_pivot = jVal;
}

bool converged(double ** matrix0, double ** matrix1, int N)
{
    return fabs(offMatrix(matrix0, N) - offMatrix(matrix1, N)) <= G_JACOBI_EPSILON;
}

ret_codes_t get_diagonal_from_matrix(double ** eigenvalues, double ** new_matrix, int N)
{
    ret_codes_t return_code = SUCCESS;
    double * local_eigenvalues = NULL;
    int i;

    local_eigenvalues = (double *)malloc(sizeof(local_eigenvalues) * N);

    if (NULL == local_eigenvalues) {
        return_code = ERROR;
        goto l_exit;
    }

    for (i = 0; i < N; i++) {
        local_eigenvalues[i] = new_matrix[i][i];
    }

    *eigenvalues = local_eigenvalues;

l_exit:
    return return_code;
}

void align_neg_zero(double * eigenvalues, double ** eigenvectors_matrix, int N)
{
    int i, j;

    for (i = 0; i < N; ++i) {
        if (-0.0 == eigenvalues[i]) {
            for (j = 0; j < N; ++j) {
                eigenvectors_matrix[i][j] *= -1;
            }
        }
    }
}

void print_vector(double * vector, int N)
{
    int i = 0;

    for (i = 0; i < (N - 1); ++i) {
        printf("%.4f,", vector[i]);
    }
    printf("%.4f\n", vector[i]);
}
