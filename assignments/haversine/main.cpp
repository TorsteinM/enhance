#include <iostream>
#include <stdio.h>
#include <string>
#include <map>


#include "sphere_point.h"

// Include the reference haversine function
#include "haversine.h"

int main(int argc, char** argv) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <uniform/cluster/converted> <random_seed> <number_of_pairs>\n";
        return 1;
    }

    std::string method(argv[1]);
    std::map<std::string, Method> allowedMethods = {{"uniform", Method::UNIFORM}, {"cluster", Method::CLUSTER}, {"converted", Method::CONVERTED}};

    if (allowedMethods.find(method) == allowedMethods.end()) {
        std::cerr << "Invalid method. Choose between 'uniform' or 'cluster'.\n";
        return 1;
    }

    std::string seed(argv[2]);
    for(char c : seed){
        if(!std::isdigit(c)){
            std::cerr << "Invalid random seed. Seed must be a positive integer.\n";
            return 1;
        }
    }

    std::string pairs(argv[3]);
    for(char c : pairs){
        if(!std::isdigit(c)){
            std::cerr << "Invalid number of pairs. It must be a positive integer.\n";
            return 1;
        }
    }

    std::cout << "You selected the following parameters:\n"
              << "Method: " << method << "\n"
              << "Random seed: " << seed << "\n"
              << "Number of pairs: " << pairs << "\n";

    // Parse arguments to ints
    u64 parsedSeed = std::stoull(seed);
    u64 parsedN = std::stoull(pairs);
    // make distance array of f64 floats
    f64* distances = (f64*)malloc(sizeof(f64) * parsedN);
    // Generate array of sphere points
    point_pair* points = generate_sphere_points(parsedN, distances, parsedSeed, allowedMethods[argv[1]]);
    
    // Write the points to file
    f64 expected_average = distances_and_average_to_file(distances, parsedN, parsedSeed);
    // Writes the average to console
    printf("Average distance: %f\n", expected_average);

    // Write the points to file
    buffered_write_pairs_json_to_file(points, parsedN, parsedSeed, method.c_str());
    // Free the memory allocated for the array of sphere points
    free(points);
    free(distances);

    return 0;
}
