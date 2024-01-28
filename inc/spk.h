#ifndef KMEANS_H
#define KMEANS_H
/* #define _GNU_SOURCE */

/*** Includes *********************************************************/
#include<stdlib.h>
#include<stdio.h>

#include "common.h"

/*** Prototypes *******************************************************/
/*** function
 * name: update_centroid
 * params: TODO fill this
 *        - IN cluster_id: TODO explain cluster_id here
 * return: None.
 */
void update_centroid(int cluster_id, DatapointList_t data, datapoint_t * centroids, int dim_count);
void divide_centroid(datapoint_t centroid, int count, int dim_count);
void addition_to_centroid(datapoint_t centroid, DatapointNode_t datanode, int dim_count);
void print_centroids (datapoint_t * centroids, int K, int dim_count);
int compare_centroids(datapoint_t *, datapoint_t *, int, int);
void distance(datapoint_t datapoint, datapoint_t centroid, int dim_count, double * dist_res);
void update_right_cluster_idx(DatapointNode_t datapoint, datapoint_t * centroids, int K, int dim_count);
void clean_centroids(datapoint_t ** centroids, int K);

ret_codes_t kmeans(DatapointList_t, datapoint_t *, int);

#endif /* KMEANS_H */
