#ifndef SPHERE_POINT_H
#define SPHERE_POINT_H


typedef unsigned long long u64;
typedef double f64;

enum class Method {
    UNIFORM,
    CLUSTER,
    CONVERTED
};


/**
 * @struct sphere_point
 * @brief A struct to represent two angles designating a point on a sphere.
 * @param x0: longitude of point 0.
 * @param y0: latitude of point 0.
 * @param x1: longitude of point 1.
 * @param y1: latitude of point 1.
 */
typedef struct {
    f64 x0;
    f64 y0;
    f64 x1;
    f64 y1;
} point_pair;


/**
 * @brief Generates an array of point pairs with the given method, 
 * and calculates the haversine distance between the points.
 *
 * @param n Amount of pairs to generate.
 * @param seed Random seed.
 * @param method Method enum to switch which method to use for generating the points.
 * @return Pointer to the array of point pairs.
 */
point_pair* generate_sphere_points(u64 n, f64* distances, u64 seed, Method method);

/**
 * @brief Generates points using uniform distribution. 
 * This method will generate random angles, 
 * which will have the side effect of placing more points towards the poles.
 */
point_pair* generate_sphere_points_uniform(u64 n, f64* distances, u64 seed);

/**
 * @brief Generates points based on picking random areas to cluster points on. 
 * This method will have a more uniform distribution of points on the sphere.
 */
point_pair* generate_sphere_points_cluster(u64 n, f64* distances, u64 seed);

 /**
 * @brief Generates points using point conversion.
 *  https://mathworld.wolfram.com/SpherePointPicking.html
 */
point_pair* generate_sphere_points_converted(u64 n, f64* distances, u64 seed);

/**
 * 
 * @brief Writes the distances of the point pairs to a file and at last the average distance.
 *  @param pairs Pointer to the array of point pairs.
 *  @param n Amount of pairs in the list.
 *  @param seed Name of the file will be ../temp/data_<seed>_haveranswer.f64.
 * @return The average distance of the point pairs.
*/
f64 distances_and_average_to_file(f64* distances, u64 n, u64 seed);

/**
 * 
 * @brief Writes the point pairs to a file.
 *  @param pairs Pointer to the array of point pairs.
 *  @param n Amount of pairs in the list.
 *  @param seed Name of the file will be ../temp/data_<seed>_<method>.json.
*/
void write_pairs_json_to_file(point_pair* pairs, u64 n, u64 seed, const char* method_name);

/**
 * 
 * @brief Writes the point pairs to a file with a buffered write.
 *  @param pairs Pointer to the array of point pairs.
 *  @param n Amount of pairs in the list.
 *  @param seed Name of the file will be ../temp/data_<seed>_<method>.json.
*/
void buffered_write_pairs_json_to_file(point_pair* pairs, u64 n, u64 seed, const char* method_name);

#endif //SPHERE_POINT_H