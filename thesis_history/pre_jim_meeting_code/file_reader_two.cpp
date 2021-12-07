//convert 3 solution to cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> //requires c++17
#include <cstdlib> // for exit()
#include <set>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "classes_two.cpp"


using namespace std; //pretty much only using this line for strings

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

//helper function to extract 2nd number from pair
int get_second_number(std::string &s){
    bool q = false;
    bool z = false;
    string x;
    for (char const qqq: s){
        string c = " ";
        c = qqq;
        bool white_only = is_number(c);

        if (q && z){
            x+=c;
        }
        else if (white_only && !q) {q = true;}
        
        else if (q && !white_only){
            z = true;
            x+=c;
            }
    }
    return std::stoi(x);
}

std::vector<int> splitter(std::string &s, std::string delim) {
    std::stringstream test(s);
    string segment;
    std::vector<int> seglist;
    int one;
    int two;

    while(std::getline(test, segment))
    {
        one = std::stoi(segment.c_str());
        two = get_second_number(segment);
        seglist.push_back(one);
        seglist.push_back(two);
        return seglist;
    }
    return seglist; //redundant to shut up error
}

void read_edges(string file,
                std::set<std::pair<int,int> > &edge_set,
                std::unordered_map<int,Vertex> &vert_map, 
                std::unordered_map<int, std::set<int> > &node_map)
{
    ifstream inf (file.c_str());
    if (!inf)
    {
        // Print an error and exit
        cerr << "Uh oh, file could not be opened for reading!" << endl;
        exit(1);
    }
    int new_id = 0;
    std::map<int,int> id_mapping;
    int vp_1;
    int vp_2;

    while (inf)
    {
        string strInput;
        getline(inf, strInput);
        if (strInput.empty()) {return;}
        std::pair<int,int> nodes;
        std::vector<int> spit = splitter(strInput, " ");
        int node_1 = spit[0]; 
        int node_2 = spit[1];
        Vertex vertex_1;
        Vertex vertex_2;  
        bool seen_you_before_1 = false;
        bool seen_you_before_2 = false;
       

        //next two if statements, to serialize values (random id -> [0 -> (n-1)])
        if (id_mapping.count(node_1) > 0){
            node_1 = id_mapping[node_1];
            vertex_1 = vert_map[node_1];
            seen_you_before_1 = true;
            
        }
        else {
            id_mapping[node_1] = new_id;
            vertex_1.id = new_id;
            new_id++;
        }

        if (id_mapping.count(node_2) > 0){
            node_2 = id_mapping[node_2];
            vertex_2 = vert_map[node_2];
            seen_you_before_2 = true;
        }
        else {
            id_mapping[node_2] = new_id;
            vertex_2.id = new_id;
            new_id++;
        }

        //sort nodes so we don't have repeats in edge_set
        if (vertex_1.id > vertex_2.id){
            Vertex temp = vertex_1;
            vertex_1 = vertex_2;
            vertex_2 = temp;
            bool temp_b = seen_you_before_1;
            seen_you_before_1 = seen_you_before_2;
            seen_you_before_2 = temp_b;
        }
        vp_1 = vertex_1.id; //pointer to vertex_1 object
        vp_2 = vertex_2.id;

        if (!seen_you_before_1){
            vert_map[vp_1] = vertex_1;
        }
        
        if (!seen_you_before_2){
            vert_map[vp_2] = vertex_2; 
        }
        
        nodes = std::make_pair(vp_1, vp_2);
        edge_set.insert(nodes);
        node_map[vp_1].insert(vp_2);
        node_map[vp_2].insert(vp_1);
    }
}


void create_graph(std::string file, Graph &new_graph){


    read_edges(file,new_graph.edge_set ,new_graph.verticies, new_graph.node_map);
    new_graph.n = new_graph.verticies.size();
    std::vector<bool> initial (new_graph.n);
    new_graph.adj = std::vector<std::vector<bool> > (new_graph.n, initial);
    for (int one = 0; one < new_graph.n; one++){
        
        for (int two : new_graph.node_map[one]){
            new_graph.adj[one][two] = true;
            new_graph.adj[two][one] = true;
        }
    }
    return;
}


std::shared_ptr<Trie> create_trie(){
    std::shared_ptr<Trie> root = std::shared_ptr<Trie>(new Trie());
    root -> n = 0; root -> adj = {};
    std::shared_ptr<Trie> node = std::shared_ptr<Trie>(new Trie());
    node -> n = 1; node -> adj = {1};

    std::shared_ptr<Trie> edge = std::shared_ptr<Trie>(new Trie());
    edge -> n = 2; edge -> adj = {1,0};

    std::shared_ptr<Trie> triangle = std::shared_ptr<Trie>(new Trie());
    triangle -> n = 3; triangle -> adj = {1,1,0}; triangle ->is_Graph = true;
    triangle-> name = "triangle";

    std::shared_ptr<Trie> two_star = std::shared_ptr<Trie>(new Trie());
    two_star -> n = 3; two_star -> adj = {1,0,0}; two_star ->is_Graph = true;
    two_star -> name = "two_star";

    //4
    std::shared_ptr<Trie> clique = std::shared_ptr<Trie>(new Trie());
    clique -> n = 4; clique -> adj = {1,1,1,0}; clique -> is_Graph = true;
    clique -> name = "clique";

    std::shared_ptr<Trie> chord_cycle = std::shared_ptr<Trie>(new Trie());
    chord_cycle -> n = 4; chord_cycle -> adj = {1,1,1,0}; chord_cycle -> is_Graph = true;
    chord_cycle -> name = "chord_cycle";

    std::shared_ptr<Trie> t_triangle = std::shared_ptr<Trie>(new Trie());
    t_triangle -> n = 4; t_triangle -> adj = {1,0,0,0}; t_triangle -> is_Graph = true;
    t_triangle -> name = "t_triangle";

    std::shared_ptr<Trie> three_star = std::shared_ptr<Trie>(new Trie());
    three_star -> n = 4; three_star -> adj = {1,0,0,0}; three_star -> is_Graph = true;
    three_star -> name = "three_star";

    std::shared_ptr<Trie> four_cycle = std::shared_ptr<Trie>(new Trie());
    four_cycle -> n = 4; four_cycle -> adj = {1,1,0,0}; four_cycle -> is_Graph = true;
    four_cycle -> name = "four_cycle";

    std::shared_ptr<Trie> four_path = std::shared_ptr<Trie>(new Trie());
    four_path -> n = 4; four_path -> adj = {0,1,0,0}; four_path -> is_Graph = true;
    four_path -> name = "four_path";
    //children
    root->children.push_back(node);
    //root.children.push_back(std::make_shared<Trie>(node));
    node->children.push_back(edge);
    edge->children.push_back(triangle);
    edge->children.push_back(two_star);
    two_star->children.push_back(four_cycle);
    two_star->children.push_back(three_star);
    two_star->children.push_back(four_path);
    triangle->children.push_back(clique);
    triangle->children.push_back(chord_cycle);
    triangle->children.push_back(t_triangle);
    //create adjacency matricies here
    //can get children from object, but not from pointer to object

    return root;
}


/*
Trie root; root.n = 0;
    Trie node; node.n = 1;
    Trie edge; edge.n = 2;
    //3-case
    Trie two_star; two_star.n = 3;
    Trie triangle; triangle.n = 3;
    //4-case
    Trie clique; clique.n = 4;  clique.is_Graph = true;
    Trie chord_cycle; chord_cycle.n = 4; chord_cycle.is_Graph = true;
    Trie t_triangle; t_triangle.n = 4; t_triangle.is_Graph = true;
    Trie four_cycle; four_cycle.n = 4; four_cycle.is_Graph = true;//ts
    Trie three_star; three_star.n = 4; three_star.is_Graph = true;//ts
    Trie four_path; four_path.n = 4; four_path.is_Graph = true;//ts
*/