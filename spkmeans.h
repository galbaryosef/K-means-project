#ifndef SPKMEANS_H
#define SPKMEANS_H
/*** Includes *********************************************************/
#include "common.h"

/*** Prototypes *******************************************************/
ret_codes_t parse_args(int argc, char *argv[], char ** goal, char ** file_name);
void count_dims(const char * line, size_t len, int * dim_count);
ret_codes_t parse_data_from_input(char * file_name, DatapointList_t data, int *dim_count);
#endif  /* SPKMEANS_H */
