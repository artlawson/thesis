//convert 3 solution to cpp

#include <fstream>
#include <string>
#include <filesystem> //requires c++17
#include <cstdlib> // for exit()
#include <set>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "algorithms_three.cpp"
#include <iterator>


using namespace std;

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

//helper function to extract 2nd number from pair
int get_second_number(std::string &s){
    bool q = false;
    bool z = false;
    std::string x;
    for (char const qqq: s){
        std::string c = " ";
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
    std::string segment;
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
                std::set<std::pair<Vertex*,Vertex*> > &edge_set,
                std::vector<Vertex> vert_vec)
{
    ifstream inf (file.c_str());
    if (!inf)
    {
        // Print an error and exit
        cerr << "Uh oh, file could not be opened for reading!" << endl;
        exit(1);
    }
    int new_id = 1;
    std::map<int,int> id_mapping;
    std::map<int, Vertex> verticies;

    while (inf)
    {
        std::string strInput;
        getline(inf, strInput);
        if (strInput.empty()) {return;}
        std::pair<Vertex*,Vertex*> nodes;
        std::vector<int> spit = splitter(strInput, " "); //currently, secon
        int node_1 = spit[0]; 
        int node_2 = spit[1];
        Vertex vertex_1;
        Vertex vertex_2;  
        bool seen_you_before_1 = false;
        bool seen_you_before_2 = false;

        //next two if statements, to serialize values (random id -> [1-n])
        if (id_mapping.count(node_1) > 0){
            node_1 = id_mapping[node_1];
            vertex_1 = verticies[node_1]; //need to make sure this and else statement assignment are equal
            seen_you_before_1 = true;
        }
        else {
            id_mapping[node_1] = new_id;
            vertex_1.id = new_id;
            verticies[new_id] = vertex_1; //could change this to be reference, but this clearly won't be bottleneck of entire code
            new_id++;
        }

        if (id_mapping.count(node_2) > 0){
            node_2 = id_mapping[node_2];
            vertex_2 = verticies[node_2]; //need to make sure this and else statement assignment are equal
            seen_you_before_2 = true;
        }
        else {
            id_mapping[node_2] = new_id;
            vertex_2.id = new_id;
            verticies[new_id] = vertex_2; //could change this to be reference, but this clearly won't be bottleneck of entire code
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

        nodes = std::make_pair(&vertex_1, &vertex_2);

        //maybe want node_set?
        //this currently isn't going to work, because regardless of whether or not we already have that vertex id, we still create new one
        edge_set.insert(nodes); //insert into set of edges
        vertex_1.neighbors.push_back(&vertex_2); //DOUBLE CHECK THIS PLZ GOD
        vertex_2.neighbors.push_back(&vertex_1);
        
        if (!seen_you_before_1){
            vert_vec.push_back(vertex_1);
        }
        if (!seen_you_before_2){
            vert_vec.push_back(vertex_2);
        }

    }
    std::cout<< "---Done initializing---" <<std::endl; 
    
}


Graph creat_graph (string file){
    Graph new_graph;

    read_edges(file, new_graph.edge_set, new_graph.verticies);
    new_graph.n = new_graph.verticies.size();
    new_graph.adj = std::vector<std::vector<bool> > (new_graph.n);
    //need to make sure we aare properly filling out adjacency list
    int one, two;
    for (Vertex v : new_graph.verticies){
        one = v.id;
        for (Vertex* next_door : v.neighbors){
            two = next_door->id;
            new_graph.adj[one][two] = true;
            new_graph.adj[two][one] = true;
        }
    }

    std::cout<< "edges have been loaded" <<std::endl;
    //now we need to figure out what we want to do with adjacency matrix
    return new_graph;
}