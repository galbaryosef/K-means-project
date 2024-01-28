/*** Includes *********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

/*** functions ********************************************************/
ret_codes_t init_datapoint_node(DatapointNode_t * node, datapoint_t value) {
    ret_codes_t ret_code = SUCCESS;

    /* allocate space in the memory for DatapointNode_s (fields: next, value, cluste_id)
     * and we are recieveing an address from malloc to the pointer (node*)
     * */
    *node = (DatapointNode_t)malloc(sizeof(**node));

    if (NULL == (*node)){
        ret_code = ERROR;
        goto l_exit;
    }

    (*node)->next = NULL;
    (*node)->value = value;
    (*node)->cluster_id = -1;

l_exit:
    return ret_code;
}

ret_codes_t init_datapoint_list(DatapointList_t * list) {
    ret_codes_t ret_code = SUCCESS;

    *list = (DatapointList_t)malloc(sizeof(**list));

    if (NULL == (*list)){
        ret_code = ERROR;
        goto l_exit;
    }

    (*list)->head = NULL;
    (*list)->length = 0;
    (*list)->dim_count = 0;

l_exit:
    return ret_code;
}

ret_codes_t append_to_list(DatapointList_t list, datapoint_t value)
{
    DatapointNode_t cur_node = list->head;
    DatapointNode_t node = NULL;

    ret_codes_t ret_code = SUCCESS;
    ret_code = init_datapoint_node(&node, value);
    if (ret_code) goto l_exit;

    if (NULL == list->head) list->head = node;
    else {
        while (NULL != cur_node->next) cur_node = cur_node->next;
        cur_node->next = node;
    }

    node->next = NULL;
    ++(list->length);

    l_exit:
    return ret_code;
}

void destroy_node(DatapointNode_t * node) {
    if (NULL != *node) {
        if (NULL != (*node)->value) {
            free((*node)->value);
            (*node)->value = NULL; /*set to NULL to declare it was already released (freed)*/
        }
        free(*node);
        *node = NULL;
    }
}

void destroy_list(DatapointList_t * list) {
    if (NULL != *list) {
        DatapointNode_t cur_node = (*list)->head;
        DatapointNode_t last_node = NULL;

        while (NULL != cur_node) {
            last_node = cur_node;
            cur_node = cur_node->next;
            destroy_node(&last_node);
        }
        
        free(*list);
        *list = NULL;
    }
}

void print_list(DatapointList_t list)
{
    DatapointNode_t cur_node = list->head;
    int i;

    printf("List length: %d\n", (int)list->length);

    while (cur_node != NULL) {
        for (i = 0; i < list->dim_count; ++i) {
            if (i == (list->dim_count - 1)) {
                printf("%f\n", cur_node->value[i]);
            } else {
                printf("%f,", cur_node->value[i]);
            }
        }
        cur_node = cur_node->next;
    }
}

