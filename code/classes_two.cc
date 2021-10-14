//classes
#include <iostream>
#include <vector>
#include <set>
#include <map>


class Vertex{
    public:
        int id; 
        std::vector<Vertex*> neighbors;
        
        ~Vertex(){
            //destructor
            for (Vertex* n : neighbors){
                delete n;
            }
        }

};
//Graph
    //initializer from make edges
    //make adjacency matrix
class Graph {
    public:
        int n; //number of verticies
        std::vector<Vertex> verticies;
        std::set<std::pair<Vertex*,Vertex*> > edge_set;
        std::vector<std::vector<bool> > adj; //vert.id   
     
        ~Graph(){
            //destructor
            for (std::pair<Vertex*,Vertex*> pair : edge_set){
                delete pair.first;
                delete pair.second;
                //delete pair; ?????
            }
        }
};

//Trie object
class Trie{
    public:
        Trie *root;
        std::vector<Trie*> children;
        int n; //number of verticies in node
        std::vector<bool> adj;
        bool is_Graph;

        ~Trie(){

            delete root;
            for (int i = 0; children.size(); i++){
                delete children[i];  
            } 
        }
     
};




