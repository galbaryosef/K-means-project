/*** Includes *********************************************************/
#include<string.h>
#include<math.h>

#include "common.h"
#include "spk.h"

/*** functions ********************************************************/
void clean_centroids(datapoint_t ** centroids, int K)
{
    int i = 0;

    if (NULL != *centroids) {
        for (; i < K; ++i) {
            if (NULL != (*centroids)[i]) {
                free((*centroids)[i]);
                (*centroids)[i] = NULL;
            }
        }
        free(*centroids);
        *centroids = NULL;
    }
}

ret_codes_t create_centroids(DatapointList_t data, int K, datapoint_t * centroids)
{
    ret_codes_t ret_code = SUCCESS;
    DatapointNode_t cur_node = data->head;
    int i = 0;
    int cur_dim = 0;

    for (; i < K; ++i) {
        centroids[i] = (datapoint_t)malloc(sizeof(**centroids) * data->dim_count);
        if (centroids[i] == NULL){
            ret_code = ERROR;
            goto l_exit;
        }
        for (cur_dim = 0; cur_dim < data->dim_count; ++cur_dim) {
            centroids[i][cur_dim] = cur_node->value[cur_dim];
        }
        cur_node = cur_node->next;
    }

l_exit:
    return ret_code;
}

void update_right_cluster_idx(DatapointNode_t datapoint, datapoint_t * centroids, int K, int dim_count)
{
    int cluster_id = -1;
    int i;
    double min;

    for(i = 0; i < K; ++i) {
        double dist_res;
        distance(datapoint->value, centroids[i], dim_count, &dist_res);
        if (cluster_id == -1){
            min = dist_res;
            cluster_id = i;
        }
        else if (dist_res < min){
            min = dist_res;
            cluster_id = i;
        }
    }
    datapoint->cluster_id = cluster_id;
}

void distance(datapoint_t datapoint, datapoint_t centroid, int dim_count, double * dist_res)
{
    double result = 0;
    int i;

    for (i = 0; i < dim_count; ++i){
        result = result + pow(datapoint[i] - centroid[i], 2);
    }

    *dist_res = pow(result, 0.5);
}

int
compare_centroids(
        datapoint_t * centroids1,
        datapoint_t * centroids2,
        int K,
        int dim_count)
{
    int are_equal = 1;
    int i = 0;
    double dist_res;

    for (; i < K; ++i) {
        distance(centroids1[i], centroids2[i], dim_count, &dist_res);
        if ((dist_res) >= G_KMEANS_EPSILON) {
            are_equal = 0;
            break;
        }
    }
    return are_equal;
}

void print_centroids(datapoint_t * centroids, int K, int dim_count)
{
    int i;
    int j;

    for (i = 0; i < K; i++) {
        for (j = 0; j < dim_count; j++) {
            if (j < dim_count - 1) {
                printf("%0.4f,",centroids[i][j]);
            } else {
                printf("%0.4f\n",centroids[i][j]);
            }
        }
    }
}

void addition_to_centroid(datapoint_t centroid, DatapointNode_t datanode, int dim_count)
{
    int i;

    for (i = 0; i < dim_count; i++){
        centroid[i] = centroid[i] + datanode->value[i];
    }
}

void divide_centroid(datapoint_t centroid, int count, int dim_count)
{
    int i;

    for (i = 0; i < dim_count; i++) {
        centroid[i] = centroid[i] / count;
    }
}

void
update_centroid(
        int cluster_id,
        DatapointList_t data,
        datapoint_t * centroids,
        int dim_count)
{
    int i;
    int count = 0;
    DatapointNode_t cur_node = data->head; /*the first node that we check*/
    datapoint_t cur_centroid = centroids[cluster_id]; /*the centroid that needs to be updated*/

    for (i = 0; i < dim_count; ++i) { /*set to 0 the centroid that needs to be updated*/
        cur_centroid[i] = 0;
    }

    for (i = 0; i < data->length; ++i){
        if (cur_node->cluster_id == cluster_id){ /*if the curent node is part of the cluster that we need to update it's centroid*/
            addition_to_centroid(cur_centroid, cur_node, dim_count);
            ++count;
        }
        cur_node = cur_node->next;
    }

    divide_centroid(cur_centroid, count, dim_count);
}

ret_codes_t kmeans(DatapointList_t data, datapoint_t * centroids, int K)
{
    ret_codes_t ret_code = SUCCESS;
    DatapointNode_t cur_node = NULL;
    datapoint_t * last_centroids = NULL; /* The centroids of each cluster */
    int curr_iter = 0;
    int cur_cluster_id;
    int dim_count;
    int i;

    dim_count = data->dim_count;

    if (K > data->length || K <= 1) {
        ret_code = ERROR;
        goto l_exit;
    }

    last_centroids = (datapoint_t *)malloc(sizeof(*last_centroids) * K);
    if (last_centroids == NULL){
            ret_code = ERROR;
            goto l_exit;
    }

    for (i = 0; i < K; ++i) {
        last_centroids[i] = (datapoint_t)malloc(sizeof(**last_centroids) * dim_count);
        if (last_centroids[i] == NULL){
            ret_code = ERROR;
            goto l_exit;
        }
    }

    for (; curr_iter < G_KMEANS_MAX_ITER; ++curr_iter) {

        cur_node = data->head;
        while (NULL != cur_node) {
            update_right_cluster_idx(cur_node, centroids, K, dim_count);
            cur_node = cur_node->next;
        }

        for (cur_cluster_id = 0; cur_cluster_id < (int)K; ++cur_cluster_id) {
            for (i = 0; i < dim_count; ++i) {
                last_centroids[cur_cluster_id][i] = centroids[cur_cluster_id][i];
            }
            update_centroid(cur_cluster_id, data, centroids, dim_count);
        }

        if (compare_centroids(last_centroids, centroids, K, dim_count)) break;
    }

l_exit:
    clean_centroids(&last_centroids, K);

    return ret_code;
}
