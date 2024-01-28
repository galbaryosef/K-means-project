#ifndef DATAPOINT_LIST_H
#define DATAPOINT_LIST_H
/*** Includes *********************************************************/
#include "datapoint.h"

/*** Structs **********************************************************/
typedef struct DatapointNode_s * DatapointNode_t;
typedef struct DatapointList_s * DatapointList_t;

struct DatapointNode_s {
    DatapointNode_t next;
    datapoint_t value;
    int cluster_id;
};

struct DatapointList_s {
    DatapointNode_t head;
    int length;
    int dim_count;
};

/*** Prototypes *******************************************************/
void destroy_list(DatapointList_t * list);
void destroy_node(DatapointNode_t * node);
ret_codes_t append_to_list(DatapointList_t list, datapoint_t value);
ret_codes_t init_datapoint_list(DatapointList_t * list);
void print_list(DatapointList_t list);
#endif  /* DATAPOINT_LIST_H */
