#include <iostream>
#include <fstream>
#include <cmath>
#include "plane.hpp"
#include "box.hpp"
#include "cone.hpp"
#include "sphere.hpp"
#include "../utils/figure.hpp"

int main(int argc, char* argv[]) {
    // Check the number of command-line arguments
    if (argc != 6) {
        std::cerr << "Usage: generator <primitive_type> <params...> <output_file>" << std::endl;
        return 1;
    }

    // Extract command-line arguments
    std::string primitive_type = argv[1];
    std::string output_file = argv[argc - 1];

    // Create a figure object
    FIGURE figure;

    // Check the primitive type and generate the corresponding shape
    if (primitive_type == "plane" && argc == 6) {
        int length = std::stoi(argv[2]);
        int divisions = std::stoi(argv[3]);
        figure = generate_plane(length, divisions);
    } else if (primitive_type == "box" && argc == 6) {
        float length = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);
        figure = generate_box(length, divisions);
    } else if (primitive_type == "sphere" && argc == 6) {
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        figure = generate_sphere(radius, slices, stacks);
    } else if (primitive_type == "cone" && argc == 6) {
        float bottom_radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        figure = generate_cone(bottom_radius, height, slices, stacks);
    } else {
        std::cerr << "Invalid arguments or primitive type." << std::endl;
        return 1;
    }

    // Save the generated figure to the output file
    save_file(figure, output_file);

    return 0;
}
