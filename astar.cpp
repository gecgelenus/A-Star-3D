#include <iomanip>

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>

#include <cmath>

#include <vector>
#include <chrono>
#include <thread>

#define OPEN 1
#define CLOSE 2


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
void findWay(node* start, node* end, bool updateNodes);
void printWay(node* target);
void drawWay(node* target, std::vector<std::string>& entityNames, std::string& color);
void resetWays();

void readFile(std::string&);


bool iterNeighbor(node*, node*, int);

node* findNode(float x, float y, float z);

float distance(node* n, node* m);


std::vector<node*> open;
std::vector<node*> close;
std::vector<node*> used;
std::vector<node*> nodeList;


int main(int argc, char** argv)
{   


    if(argc != 2){
        std::cout << "Invalid arguments given. Use --help for help" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];


    if(fileName == "--help" || fileName == "-h"){
        std::cout << "Usage: EXECUTABLE [graph file name]" << std::endl;
        return 0;
    }


    readFile(fileName);


    node* start = findNode(7.0f, 7.0f, 9.0f);
    node* end = findNode(-9.0f, -5.0f, -7.0f);
    

    findWay(start, end, true);
    printWay(end);
    resetWays();

    start = findNode(6.0f, 9.0f, 5.0f);
    end = findNode(-6.0f, -9.0f, -9.0f);

    findWay(start, end, true);
    printWay(end);
    resetWays();
}





bool iterNeighbor(node* cur, node* next, int cost) {

    for (node* n : close) {
        if (n->id == cur->id)
            return false;
    }

    int temp_g = cur->g + cost;

    if (temp_g > next->g) {
        return false;
    }

    next->g = temp_g;
    next->f = next->g + next->h;
    next->previous = cur;

    bool found = false;

    for (node* n : open) {
        if (n->id == next->id) {
            found = true;
        }
    }


    if (!found) {
        open.push_back(next);
    }


}


float distance(node* n, node* m) {
    return sqrt((n->x - m->x) * (n->x - m->x) + (n->y - m->y) * (n->y - m->y) +
        (n->z - m->z) * (n->z - m->z));

}


void readFile(std::string& fileName){

    std::cout << "Reading file, current node list size: " << nodeList.size() << std::endl;
    std::ifstream file;
    file.open(fileName);

    char buffer[1024];

    while(file.getline(buffer, 1024)){
        std::stringstream line(buffer);
        std::vector<std::string> words;
        std::string temp;
        words.reserve(50);

        while(line >> temp){
            words.push_back(temp);
        }

        if(words[0] == "NL"){
            node* n = nodeList[std::stod(words[1])];
            int count = std::stod(words[2]);
            for(int i = 3; i < (count*2)+3; i += 2){
                n->neighbors.push_back(nodeList[std::stod(words[i])]);
                n->cost.push_back(std::stof(words[i+1]));
            }


        }else{
            node* n = new node();
            n->id = std::stod(words[0]);
            n->x = std::stof(words[1]);
            n->y = std::stof(words[2]);
            n->z = std::stof(words[3]);
            nodeList.push_back(n);
            
        }

        

    }

    


}

void findWay(node* start, node* target, bool updateNodes){

    for (node* n : nodeList) {
        n->h = distance(n, target);
    }


    start->g = 0;
    open.push_back(start);

    
    while (true) {

        node* min;
        min = open[0];

        for (node* n : open) {
            if (min->f > n->f) {
                min = n;
            }
        }

        auto it = std::find(open.begin(), open.end(), min);

        if (it != open.end()) {
            open.erase(it);
        }


        if (min->id == target->id) {
            std::cout << "Target found" << std::endl;
            start->g = 999999999;
            break;
        }

        for (int i = 0; i < min->neighbors.size(); i++) {

            iterNeighbor(min, min->neighbors[i], min->cost[i]);


        }

        close.push_back(min);


    }

if(updateNodes){
    node* cur = target;

    while (cur->previous != nullptr) {
        used.push_back(cur);
        cur->used = true;
        cur = cur->previous;
    }
    used.push_back(cur);
    cur->used = true;
}
    close.clear();
    open.clear();

if(updateNodes){
    for (node* u : used) {
        for (node* n : nodeList) {
            if (u->id == n->id)
                continue;

            for (int i = 0; i < n->neighbors.size(); i++) {
                if (n->neighbors[i]->id == u->id) {
                    n->neighbors.erase(n->neighbors.begin() + i);
                    n->cost.erase(n->cost.begin() + i);
                }

            }


        }

    }

    for (int i = 0; i < nodeList.size(); i++) {

        for (node* u : used) {
            if (u->id == nodeList[i]->id) {
                nodeList.erase(nodeList.begin() + i);
            }
        }


    }

}
}


node* findNode(float x, float y, float z){

    for(node* n: nodeList){
        if(n->x == x && n->y == y && n->z == z){
            return n;
        }
    }


}

void printWay(node* target){

    node* cur = target;

    while (cur->previous != nullptr) {
        std::cout << cur->id << "-> X:" << cur->x << " Y:" << cur->y << " Z:" << cur->z << std::endl;;
        cur = cur->previous;
    }

    std::cout << cur->id << "-> X:" << cur->x << " Y:" << cur->y << " Z:" << cur->z << std::endl;;
    std::cout << "----------------------------------------------------------" << std::endl;
}

void resetWays(){
    for(node* n: nodeList){
        n->previous == nullptr;
    }
}


void drawWay(node* target, std::vector<std::string>& entityNames, std::string& color){

    node* cur = target;
    int counter = 1;

    while (cur->previous != nullptr) {

        std::stringstream ss("");

        ss << "drawline line" << counter << " " <<
            cur->x << std::fixed << std::setprecision(2) << "f " <<
            cur->y << std::fixed << std::setprecision(2) << "f " <<
            cur->z << std::fixed << std::setprecision(2) << "f " <<
            cur->previous->x << std::fixed << std::setprecision(2) << "f " <<
            cur->previous->y << std::fixed << std::setprecision(2) << "f " <<
            cur->previous->z << std::fixed << std::setprecision(2) << "f " <<
            "1.0f 0.0f 0.0f 1.0f" << std::endl;
        //send(ConnectSocket, ss.str().c_str(), ss.str().size(), 0);
        entityNames.push_back("line" + counter);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cur = cur->previous;
        counter++;
    }


    cur = target;

    counter = 1;

    while (cur->previous != nullptr) {

        std::stringstream ss("");

        ss << "add wp" << counter << " 0.2f " <<
            cur->x << std::fixed << std::setprecision(2) << "f " <<
            cur->y << std::fixed << std::setprecision(2) << "f " <<
            cur->z << std::fixed << std::setprecision(2) << "f " <<
            color << std::endl;
        //send(ConnectSocket, ss.str().c_str(), ss.str().size(), 0);
        entityNames.push_back("wp" + counter);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        cur = cur->previous;
        counter++;
    }
    std::stringstream ss("");
    ss << "add " << "a" << counter << " 0.20f " <<
        cur->x << std::fixed << std::setprecision(2) << "f " <<
        cur->y << std::fixed << std::setprecision(2) << "f " <<
        cur->z << std::fixed << std::setprecision(2) << "f " <<
        color << std::endl;
    //send(ConnectSocket, ss.str().c_str(), ss.str().size(), 0);
    entityNames.push_back("wp" + counter);




}