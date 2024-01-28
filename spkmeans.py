import os
import copy
import csv
import math

import numpy as np

from enum import Enum
import mykmeanssp


_G_MAX_ITER = 100
_G_EPSILON = 10.0 ** -5

class Goals(Enum):
    WAM = 'wam'
    DDG = 'ddg'
    GL = 'gl'
    JACOBI = 'jacobi'
    SPK = 'spk'


def parse_file(input_file):
    """! parse an input file into a useable data.
             The data will be in a format of a list containing tuples,
             each tuple is a datapoint representing a vector.

    @param input_file (`str`): A path to the file we want to open.

    @return: This function returns the data in the agreed format from earlier.
    """
    #adding cvs to empty data
    data = []

    with open(input_file) as txt_file:
        csv_reader = csv.reader(txt_file)

        for line in csv_reader:
            data.append([float(x) for x in line])
        
    return data


def distance(centroid, vector):#calculating the distance between a vector and a centroid
    n = len(centroid)
    result = 0
    delta = [centroid[i]-vector[i] for i in range(n)]

    for x in delta:
        result += x ** 2

    return math.sqrt(result)

def calculate_min_distance(centroids, vector):
    """! calculating the min distance between a vector and all centroids.
    """
    dist = distance(centroids[0], vector)

    for cur_centroid in centroids[1:]:
        cur_dist = distance(cur_centroid, vector)
        if cur_dist <= dist:
            dist = cur_dist
    
    return dist


def create_centroids(data, K):
    """! creat a data structure that stores K centroids.
             The data will be in a format of a numpy array containing K numpy arrays,
             each sub-array is a datapoint representing a centroid of a cluster.
             dont forget to make a deepcopy of the datapoints.

    @param data (`numpy`): numpy array of datapoints.

    @return: This function returns the data in the agreed format from earlier.
    """
    all_centroids = []
    indexes = []

    randIndex = np.random.choice(len(data))
    indexes.append(randIndex)
    first_centroid = data[randIndex]  # first centroid chosen randomly
    all_centroids.append(first_centroid) 

    for i in range(K - 1):
        # creating np array with the propabilities needed based on the distance
        distances = [calculate_min_distance(all_centroids, cur_data) for cur_data in data]
        dist_sum = np.sum(distances)
        distances = [dist / dist_sum for dist in distances]
        
        # chose another centroid based on the propability
        randIndex = np.random.choice(len(data), p=distances)
        indexes.append(randIndex)
        all_centroids.append(data[randIndex])
    
    return all_centroids, indexes


def print_centroids(centroids):
    """! Print to the screen the cetroids in the format of 4 decimal places.
        every centroid is presented in a saparate line, the coordinates are saparated by spaces.

    @param centroids (`list`): A list of centroids.
    """
    lines = []
    
    for centroid in centroids:
        cents_str = []
        for x in centroid:
            cents_str.append("{:.4f}".format(x))

        lines.append(','.join(cents_str))
    print('\n'.join(lines))


def print_vector(vector):
    """! Print to the screen the cetroids in the format of 4 decimal places.
        every centroid is presented in a saparate line, the coordinates are saparated by spaces.

    @param centroids (`list`): A list of centroids.
    """
    line_str = []

    for value in vector:
        line_str.append("{:.4f}".format(value))

    print(','.join(line_str))


def print_matrix(matrix):
    print_centroids(matrix)


def print_indexes(indexes):
    print(','.join(str(index) for index in indexes))


def eigengap_heuristic(eigenvalues, N):
    difference = 0.0
    max_difference = 0.0
    index = 0
    n = math.floor(N / 2)
    
    for i in range(n):
        difference = abs(eigenvalues[i] - eigenvalues[i + 1])
        if difference > max_difference:
            max_difference = difference
            index = i

    return index + 1


def parse_args():
    if len(os.sys.argv) < 4:  # K was not supplied by the user
        goal = Goals(os.sys.argv[1])
        input_file = os.sys.argv[2]
        data = parse_file(input_file)
        K = None

    else:
        K = int(os.sys.argv[1])
        goal = Goals(os.sys.argv[2])
        input_file = os.sys.argv[3]
        data = parse_file(input_file)

    # checking whether input of K is valid.
    if goal == Goals.SPK and K is not None and not 1 <= K <= len(data):
        raise Exception("An Error Has Occurred")

    return K, data, goal


def get_eigenvectors(data, K):
    N = len(data)
    datapoint_size = len(data[0])
    laplacian = mykmeanssp.gl(data, N, datapoint_size)
    eigenvalues, eigenvectors = mykmeanssp.jacobi(laplacian, N)

    eigenvalues = np.array(eigenvalues)
    eigenvectors = np.array(eigenvectors)

    idx = eigenvalues.argsort()
    eigenvalues = eigenvalues[idx]
    eigenvectors = eigenvectors[:, idx]

    if K is None:
        K = eigengap_heuristic(eigenvalues, N)

    new_data = eigenvectors[:, :K]

    return K, new_data.tolist()


def kmeans_pp(data, K):
    centroids, indexes = create_centroids(data, K)
    result_centroids = mykmeanssp.spk(centroids, data, K, len(data), len(data[0]))
    print_indexes(indexes)
    print_centroids(result_centroids)


def main():
    np.random.seed(0)
    curr_iter = 0
    dict_datapoints = {}
    K, data, goal = parse_args()
    N = len(data)  # datapoints_count
    datapoint_size = len(data[0])  # dim_count
    
    # print_centroids(centroids)
    if goal == Goals.WAM:
        matrix = mykmeanssp.wam(data, N, datapoint_size)
        print_matrix(matrix)

    elif goal == Goals.DDG:
        matrix = mykmeanssp.ddg(data, N, datapoint_size)
        print_matrix(matrix)

    elif goal == Goals.GL:
        matrix = mykmeanssp.gl(data, N, datapoint_size)
        print_matrix(matrix)

    elif goal == Goals.JACOBI:
        eigenvalues, eigenvectors = mykmeanssp.jacobi(data, N)
        print_vector(eigenvalues)
        print_matrix(eigenvectors)

    elif goal == Goals.SPK:
        K, eigenvectors = get_eigenvectors(data, K)
        new_data = eigenvectors
        kmeans_pp(new_data, K)


if __name__ == '__main__':
    main()
