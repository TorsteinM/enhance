
#include "sphere_point.h"
#include "haversine.h"
#include "buffered_write.cpp"
#include <random>
#include <stdio.h>
#include <string>
#include <thread>
#include <sstream>

// Writing distances to file. TODO(tm): Consider refactor to a batch write.
f64 distances_and_average_to_file(f64* distances, u64 n, u64 seed){

    // Using high level methods for filename generation
    std::string filename = "../temp/data_" + std::to_string(seed) + "_haveranswer.f64";
    
    // open file using fopen_s on msvc
    FILE* file;
    #ifdef _MSC_VER
    fopen_s(&file, filename.c_str(), "wb");
    #else
    file = fopen(filename.c_str(), "wb");
    #endif

    if (file == nullptr) {
        printf("Could not open file %s\n", filename.c_str());
        return -1.0;
    }

    // write distances to file
    fwrite(distances, sizeof(f64), n, file);
    
    // calculate average distance
    f64 average = 0.0;
    for (u64 i = 0; i < n; i++) {
        average += distances[i];
    }
    average /= n;
    // write average distance to file
    fwrite(&average, sizeof(f64), 1, file);
    fclose(file);
    
    return average;

}

// A function to be executed in a separate thread
void writer_thread(circular_buffer<std::string>& buffer, const std::string& filename) {
    // Open file
    FILE* file;
    #ifdef _MSC_VER
    fopen_s(&file, filename.c_str(), "wb");
    #else
    file = fopen(filename.c_str(), "wb");
    #endif
    if (file == nullptr) {
        printf("Could not open file %s\n", filename.c_str());
        return;
    }

    fprintf(file, "[\n");

    // Get data from buffer in a loop
    while (true) {
        std::string data = buffer.get();
        // Check if thread should exit
        if (data == "__EXIT__") {
            break;
        }
        // Here data is written to the hard drive
        fprintf(file, "%s", data.c_str());
    }

    fprintf(file, "\n]");
    fclose(file);
}

void buffered_write_pairs_json_to_file(point_pair* pairs, u64 n, u64 seed, const char* method_name){
    // Initialization of the buffer
    circular_buffer<std::string> buffer(10000);

    // Using high level methods for filename generation
    std::string filename = "../temp/data_" + std::to_string(seed) + "_" + method_name + ".json";

    // Starting the writer thread
    std::thread writer(writer_thread, std::ref(buffer), filename);
    writer.detach(); // So that the writer thread can continue independently

    // Now populate the buffer using stringstream
    std::stringstream ss;
    ss << "\t{\"x0\":" << pairs[0].x0 << ",\"y0\":" << pairs[0].y0 << ",\"x1\":" << pairs[0].x1 << ",\"y1\":" << pairs[0].y1 << "}";
    buffer.put(ss.str());
    ss.str(""); // Clear the stringstream
    for (u64 i = 1; i < n; i++) {
        ss << ",\n\t{\"x0\":" << pairs[i].x0 << ",\"y0\":" << pairs[i].y0 << ",\"x1\":" << pairs[i].x1 << ",\"y1\":" << pairs[i].y1 << "}";
        buffer.put(ss.str());
        ss.str(""); // Clear the stringstream
    }

    // Notify writer thread that all data is in the buffer
    buffer.put("__EXIT__");
}



// Writing json pairs to file.
void write_pairs_json_to_file(point_pair* pairs, u64 n, u64 seed, const char* method_name){
    
        // Using high level methods for filename generation
        std::string filename = "../temp/data_" + std::to_string(seed) + "_" + method_name + ".json";

        
        // open file using fopen_s on msvc
        FILE* file;
        #ifdef _MSC_VER
        fopen_s(&file, filename.c_str(), "wb");
        #else
        file = fopen(filename.c_str(), "wb");
        #endif
    
        if (file == nullptr) {
            printf("Could not open file %s\n", filename.c_str());
            return;
        }
        // Write the points to file
        fprintf(file, "[\n\t{\"x0\":%f,\"y0\":%f,\"x1\":%f,\"y1\":%f}", pairs[0].x0, pairs[0].y0, pairs[0].x1, pairs[0].y1);
        for (u64 i = 1; i < n; i++) {
            // Write the json formatted points x0, y0, x1, y1 to file 
            fprintf(file, ",\n\t{\"x0\":%f,\"y0\":%f,\"x1\":%f,\"y1\":%f}", pairs[i].x0, pairs[i].y0, pairs[i].x1, pairs[i].y1);
        }
        fprintf(file, "\n]");
        
        fclose(file);
}

point_pair* generate_sphere_points(u64 n, f64* distances, u64 seed, Method method) {

    if (method == Method::CONVERTED) {
        return generate_sphere_points_converted(n, distances, seed);
    } else if (method == Method::UNIFORM) {
        return generate_sphere_points_uniform(n, distances, seed);
    } else if (method == Method::CLUSTER) {
        return generate_sphere_points_cluster(n, distances, seed);
    } else {
        return nullptr;
    }
}



point_pair* generate_sphere_points_uniform(u64 n, f64* distances, u64 seed){
    
    // Mersenne Twister 64-bit pseudo-random generator
    std::mt19937_64 rng(seed);
    // double distribution for longitude in range [-180, 180]
    std::uniform_real_distribution<f64> x_distribution(-180.0, 180.0);
    // double distribution for latitude in range [-90, 90]
    std::uniform_real_distribution<f64> y_distribution(-90.0, 90.0);


    // allocate memory for point pairs
    point_pair* pairs = (point_pair*)malloc(sizeof(point_pair) * n);

    // generate n point pairs
    for (u64 i = 0; i < n; i++) {
        // generate random angles
        f64 x0 = x_distribution(rng);
        f64 y0 = y_distribution(rng);
        f64 x1 = x_distribution(rng);
        f64 y1 = y_distribution(rng);

        // calculate distance
        distances[i] = ReferenceHaversine(x0, y0, x1, y1, 6372.8);

        pairs[i] = { x0, y0, x1, y1 };
    }
    return pairs;
}
point_pair* generate_sphere_points_cluster(u64 n, f64* distances, u64 seed){
        
    // Mersenne Twister 64-bit pseudo-random generator
    std::mt19937_64 rng(seed);
    // double distribution for longitude in range [-180, 180]
    std::uniform_real_distribution<f64> xCenter_distribution(-180.0, 180.0);
    // double distribution for latitude in range [-90, 90]
    std::uniform_real_distribution<f64> yCenter_distribution(-90.0, 90.0);
    // delta angle distribution for x and y direction
    std::uniform_real_distribution<f64> delta_distribution(-10.0, 10.0);

    // allocate memory for point pairs
    point_pair* pairs = (point_pair*)malloc(sizeof(point_pair) * n);
    
    // k clusters
    u64 clusterSize = 1;
    while(clusterSize*clusterSize*clusterSize < n)
        clusterSize++;
    // i
    f64 xCenter;
    f64 yCenter;
    for(u64 i = 0; i < n; i++){
        // Generate cluster center at every mxK point
        if(i % clusterSize == 0){
            xCenter = xCenter_distribution(rng);
            yCenter = yCenter_distribution(rng);
        }
        // Generate x0, y0, x1, y1 around cluster center
        f64 x0 = xCenter + delta_distribution(rng);
        f64 y0 = yCenter + delta_distribution(rng);
        f64 x1 = xCenter + delta_distribution(rng);
        f64 y1 = yCenter + delta_distribution(rng);

        // calculate distance
        distances[i] = ReferenceHaversine(x0, y0, x1, y1, 6372.8);
        // store point pair
        pairs[i] = { x0, y0, x1, y1 };
    }

    return pairs;
}
point_pair* generate_sphere_points_converted(u64 n, f64* distances, u64 seed){
    return nullptr;
}

