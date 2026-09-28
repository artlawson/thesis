//classes
// g++ -std=c++17 classes_two.cpp
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>


class Vertex{
    public:
        int id; 
        //std::vector<int> neighbors;
        
        //~Vertex(){
            //destructor
            //for (Vertex* n : neighbors){
            //    delete n;
            //}
        //}

};
//Graph
    //initializer from make edges
    //make adjacency matrix
class Graph {
    public:
        int n; //number of verticies
        //std::vector<Vertex> verticies;
        std::unordered_map<int, std::set<int> > node_map;
        std::unordered_map<int, Vertex> verticies;
        std::set<std::pair<int,int> > edge_set;
        std::vector<std::vector<bool> > adj; //vert.id   
     
        //~Graph(){
            //destructor
        //    for (std::pair<Vertex*,Vertex*> pair : edge_set){
       //         delete pair.first;
        //        delete pair.second;
                //delete pair; ?????
        //    }
        //}
};

//Trie object
class Trie{
    public:
        Trie *root;
        std::vector<Trie*> children;
        int n; //number of verticies in node
        std::vector<bool> adj;
        bool is_Graph;

        //~Trie(){

        //    delete root;
        //    for (int i = 0; children.size(); i++){
        //        delete children[i];  
        //    } 
        //}
     
};





