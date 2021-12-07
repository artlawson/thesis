//create node and edge classes

#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Edge;//forward declaration
class Node {
    
  public:
    int id;
    std::vector<Edge> edges;
    std::vector<Node> neighbors;
    void set_neighbors();
    //int area() {return width*height;}
};

class Edge {
    std::set<Node> nodes; //figure out how to pass initalized values here
  public:
    bool edge_nodes (Node node){
        return (this -> nodes.find(node) != this -> nodes.end());
    }
    Node* neighbor_helper(Node node){ //returns the 'other' node in an edge pair
        for (Node element : this -> nodes ){ //iterates through each element of set
            if (element.id != node.id){return *element;}
        }
        return nullptr;
    }

    //int area() {return width*height;}
};

void Node::set_neighbors (){
        for (int i=0; i < this -> edges.size(); i++) {
            this -> neighbors.push_back(edges[i].neighbor_helper(*this));
        }
    }


