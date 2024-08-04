#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

class node {

public:
    node() {
        status = 0;
        previous = nullptr;
        g = 999999999;
        h = 0;
        f = 0;
        name = "";
        used = false;

    }
    std::string name;
    int id;
    int status;
    float g;
    float h;
    float f;

    bool used;

    float x;
    float y;
    float z;


    node* previous;
    std::vector<node*> neighbors;
    std::vector<float> cost;


};


float distance(node* n, node* m);


int main(int argc, char** argv) {



    std::string fileName = argv[1];


    if (fileName == "--help" || fileName == "-h") {
        std::cout << "Usage: EXECUTABLE [output file name]  [Max X] [Max Y] [Max Z] [Min X] [Min Y] [Min Z] [Resolution]" << std::endl;
        return 0;
    }
    if (argc != 9) {
        std::cout << "Invalid arguments given. Use --help for help" << std::endl;
        std::cout << "Given: " << argc << std::endl;

        return -1;
    }

    std::cout << fileName << std::endl;
    float max_x = std::stof(argv[2]);
    float max_y = std::stof(argv[3]);
    float max_z = std::stof(argv[4]);

    float min_x = std::stof(argv[5]);
    float min_y = std::stof(argv[6]);
    float min_z = std::stof(argv[7]);

    float resolution = std::stof(argv[8]);

    int id_counter = 0;

    std::vector<node*> nodeList;


    for (float i = min_x; i <= max_x; i += resolution) {
        for (float j = min_y; j <= max_y; j += resolution) {
            for (float k = min_z; k <= max_z; k += resolution) { // creating nodes and assigning ids
                node* n = new node();
                n->x = i;
                n->y = j;
                n->z = k;
                n->id = id_counter;
                id_counter++;
                nodeList.push_back(n);
            }
        }

    }

    std::cout << "Node count: " << nodeList.size() << std::endl;


    int n_counter = 0;
    int temp = 1;
    for (node* n : nodeList) {
        n_counter++;
        if (((nodeList.size() / 100) * temp) < n_counter) { // percentage of work done
            std::cout << "%" << temp << std::endl;
            temp++;

        }

        for (node* m : nodeList) {
            if (n->x == (m->x + resolution) || n->x == (m->x - resolution) || n->x == m->x) {
                if (n->y == (m->y + resolution) || n->y == (m->y - resolution) || n->y == m->y) {
                    if (n->z == (m->z + resolution) || n->z == (m->z - resolution) || n->z == m->z) { // finding neighbors on all dimensions
                        if (n->id == m->id)
                            continue;

                        n->neighbors.push_back(m);
                        float cost = distance(n, m);
                        n->cost.push_back(cost);
                        continue;
                    }
                }
            }
        }
    }


    std::ofstream file;
    file.open(fileName);

    for (node* n : nodeList) {
        file << n->id << " " << n->x << std::fixed << std::setprecision(2) << " " <<
            n->y << std::fixed << std::setprecision(2) << " " <<
            n->z << std::fixed << std::setprecision(2) << std::endl; // printing nodes and heuristic distance values
    }


    for (node* n : nodeList) {
        file << "NL " << n->id << " " << n->neighbors.size() << " "; // printing neighbors
        for (int i = 0; i < n->neighbors.size(); i++) {
            file << n->neighbors[i]->id << std::fixed << std::setprecision(2) << " " << n->cost[i] << std::fixed << std::setprecision(2) << " ";
        }
        file << std::endl;
    }


    file.close();



}





float distance(node* n, node* m) {
    return sqrt((n->x - m->x) * (n->x - m->x) + (n->y - m->y) * (n->y - m->y) +
        (n->z - m->z) * (n->z - m->z));

} 
