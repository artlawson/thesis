//convert 3 solution to cpp
//gcc -lstdc++ -std=c++11  file_reader.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> //requires c++17
#include <cstdlib> // for exit()
#include <set>
#include <cstdlib>
#include <sstream>
#include <algorithm>
//#include "classes.cpp"
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <iterator>


//then parallelize it
//need to bring Stanford data in
/*
nodeId.edges : The edges in the ego network for the node 'nodeId'. Edges are undirected for facebook, and directed (a follows b) for twitter and gplus. The 'ego' node does not appear, but it is assumed that they follow every node id that appears in this file.

nodeId.circles : The set of circles for the ego node. Each line contains one circle, consisting of a series of node ids. The first entry in each line is the name of the circle.

nodeId.feat : The features for each of the nodes that appears in the edge file.

nodeId.egofeat : The features for the ego user.

nodeId.featnames : The names of each of the feature dimensions. Features are '1' if the user has this property in their profile, and '0' otherwise. This file has been anonymized for facebook users, since the names of the features would reveal private data.
*/

//ego node is friends with all of the nodes in that section
//from 

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

void read_edges(string file, std::map<int,std::set<int> > &node_map,
                std::set<std::pair<int,int> > &edge_set)
{
    ifstream inf (file.c_str());
    if (!inf)
    {
        // Print an error and exit
        cerr << "Uh oh, file could not be opened for reading!" << endl;
        exit(1);
    }

    while (inf)
    {
        std::string strInput;
        getline(inf, strInput);
        if (strInput.empty()) {return;}
        std::pair<int,int> nodes;
        std::vector<int> spit = splitter(strInput, " "); //currently, secon
        int node_1 = spit[0]; 
        int node_2 = spit[1];

        //sort nodes so we don't have repeats
        if (node_1 < node_2){
            nodes = std::make_pair(node_1, node_2);
        }
        else {nodes = std::make_pair(node_2, node_1);}
        edge_set.insert(nodes); //insert into set of edges
        node_map[node_1].insert(node_2); //add each node to each other's neighbor set
        node_map[node_2].insert(node_1);

    }
    std::cout<< "---Done initializing---" <<std::endl;
}

void triangle_finder_e(std::set<std::pair<int,int> > &edges, std::map<int, std::set<int> > &node_map,
    std::set<std::set<int> > &triangles, std::set<std::set<int> > &two_stars)
    //may want to sort based off of which node is in the middle, this could be useful
    //for informing larger data (i.e. graphlets of size 4 & 5)
{
    //do this one for iterating over Nodes
    for (std::pair<int,int> edge_1 : edges){ //for each edge
        //
        int node_1 = edge_1.first;
        int node_2 = edge_1.second;
        //need to see if they have any common nodes in their neighbor sets
        //for each one they have in  common, triangles ++
        //for each they don't, 3-stars ++
        std::set<int> set_1 = node_map[node_1];
        std::set<int> set_2 = node_map[node_2];

        for (int node_3a : set_1){
            //check if int in set 2
            std::set<int> thrupple = {node_1,node_2,node_3a}; //should be sorted already
            if (node_3a == node_1 || thrupple.size() < 3) {continue;}
            if (set_2.find(node_3a) != set_2.end()){
                triangles.insert(thrupple);
            }
            else {two_stars.insert(thrupple);}
        }

        for (int node_3b : set_2){
            //check if int in set 2
            std::set<int> thrupple = {node_1,node_2,node_3b};
            if (node_3b == node_1 || thrupple.size() < 3) {continue;}
            if (set_1.find(node_3b) != set_1.end()){
                triangles.insert(thrupple);
            }
            else {two_stars.insert(thrupple);}
        }
    }
}

//parallel 3 case
std::map< std::string, std::set<std::set<int> > > 
    triangle_finder_p(std::set<std::pair<int,int> > edges, //not sure why this doesn't work when edges is passed by reference
    std::map<int, std::set<int> > &node_map)

{    
    std::map< std::string, std::set<std::set<int> > > threes;
    //do this one for iterating over Nodes

    for (std::pair<int,int> edge_1 : edges){ //for each edge 
        //
        int node_1 = edge_1.first;
        int node_2 = edge_1.second;
        //need to see if they have any common nodes in their neighbor sets
        //for each one they have in  common, triangles ++
        //for each they don't, 3-stars ++
        std::set<int> set_1 = node_map[node_1];
        std::set<int> set_2 = node_map[node_2];

        for (int node_3a : set_1){
            //check if int in set 2
            std::set<int> thrupple = {node_1,node_2,node_3a}; //should be sorted already

            if (node_3a == node_1 || thrupple.size() < 3) {continue;}
            if (set_2.find(node_3a) != set_2.end()){
                threes["triangles"].insert(thrupple);
            }
            else {threes["two_stars"].insert(thrupple);}
        }
         
        for (int node_3b : set_2){
            //check if int in set 2
            std::set<int> thrupple = {node_1,node_2,node_3b};
            if (node_3b == node_1 || thrupple.size() < 3) {continue;}
            if (set_1.find(node_3b) != set_1.end()){
                threes["triangles"].insert(thrupple);
            }
            else {threes["two_stars"].insert(thrupple);}
        }        
    }
    return threes;
}

//4-clique sequential
void clique_four (std::set<std::pair<int,int> > &edges, std::map<int, std::set<int> > &node_map,
     std::set<std::set<int> > &cliques)
     //6 total edges in a 4-clique
{
    //do this one for iterating over Nodes
    for (std::pair<int,int> edge_1 : edges){ //for each edge
        //
        int node_1 = edge_1.first;
        int node_2 = edge_1.second; //1-2
        
        std::set<int> set_1 = node_map[node_1];
        std::set<int> set_2 = node_map[node_2];

        for (int node_3a : set_1){ //1-3
            std::set<int> set_3a = node_map[node_3a];
            
            for (int node_4a : set_3a){ //going through each node 3-> //3-4
                //now we have all 4
                //4-clique checks if they are all connected to each other
                std::set<int> set_4a = node_map[node_4a];
                std::set<int> clique = {node_1,node_2,node_3a, node_4a};
                if (clique.size() < 4) {continue;}

                if (set_4a.find(node_2) != set_4a.end() && //4-2
                    set_4a.find(node_1) != set_4a.end() && //4-1
                    set_3a.find(node_2) != set_3a.end() ) //3-2
                {
                    cliques.insert(clique);
                    //std::cout<< node_1 << " "<<node_2 <<" " << node_3a << " "
                    // << node_4a <<std::endl;
                }
            }
        }
        //checking for node_2 set
        for (int node_3b : set_2){ //1-3
            std::set<int> set_3b = node_map[node_3b];
            
            for (int node_4b : set_3b){ //going through each node 3-> //3-4
                //now we have all 4
                //4-clique checks if they are all connected to each other
                std::set<int> set_4b = node_map[node_4b];
                std::set<int> clique = {node_1,node_2,node_3b, node_4b};
                if (clique.size() < 4) {continue;}

                if (set_4b.find(node_2) != set_4b.end() && //4-2
                    set_4b.find(node_1) != set_4b.end() && //4-1
                    set_3b.find(node_1) != set_3b.end() ) //3-1
                {
                    cliques.insert(clique);
                    //std::cout<< node_1 << " "<<node_2 <<" " << node_3b << " "
                    // << node_4b <<std::endl;
                    //std::cout<< "new clique found" <<std::endl;
                }
            }
        }

    }
}

//clique parallel with triangles
std::set<std::set<int> >  clique_four_triangle (std::set<std::set<int> > triangles, 
    std::map<int, std::set<int> > &node_map )
     //6 total edges in a 4-clique
{
    //std::cout<< " clique running" << std::endl;
    std::set<std::set<int> > cliques;
    //do this one for iterating over Nodes
    for (std::set<int> tri : triangles){ //for each edge
        //
        int node_1 = *tri.begin();
        int node_2 = *next(tri.begin(),1);
        int node_3 = *next(tri.begin(),2);
        
        std::set<int> set_1 = node_map[node_1];
        std::set<int> set_2 = node_map[node_2];
        std::set<int> set_3 = node_map[node_3];
        
        //checking for node_2 set
        for (int node_4 : set_1){ //1-4
            std::set<int> clique = {node_1, node_2,node_3,node_4};
            
            if (node_4 == node_2 || node_4 == node_3 || clique.size() < 4){continue;}
    
            if (set_3.find(node_4) != set_3.end() && //3-4
                set_2.find(node_4) != set_2.end()) //2-4
            {
                cliques.insert(clique);
            }
            
        }

    }
    return cliques;
}

//chordal cycle & tailed triangles & cliques parallel with triangles
std::set<std::set<int> >  chord_cycle (std::set<std::set<int> > triangles, 
    std::map<int, std::set<int> > &node_map )
{
    std::set<std::set<int> > c_cycle;
    std::set<std::set<int> > t_triangles;
    std::set<std::set<int> > cliques;
    std::vector<bool> connections { false,false,false };

    //do this one for iterating over Nodes         
    for (std::set<int> tri : triangles){ //for each edge
        //
        int node_1 = *tri.begin();
        int node_2 = *next(tri.begin(),1); //use this to switch vectors into sets in main function
        int node_3 = *next(tri.begin(),2);
        
        std::set<int> set_1 = node_map[node_1];
        std::set<int> set_2 = node_map[node_2];
        std::set<int> set_3 = node_map[node_3];
        
        for (int node_4a : set_1){ //1-4
            connections = { true,false,false }; //[0] we are given
            std::set<int> group = {node_1, node_2,node_3,node_4a};
            if (node_4a == node_2 || node_4a == node_3 || group.size() < 4){continue;}
            
            //now we need to differentiate between the different 4-triangle shapes
            connections[2] = (set_3.find(node_4a) != set_3.end());  //3-4
            connections[1] = (set_2.find(node_4a) != set_2.end()); //2-4

            //Too tired to verify this, double check in the morning :) xxxx
            if (connections[1] && connections[2]) 
            {
                cliques.insert(group);
            }
            else if (connections[1] || connections[2])
            {
                c_cycle.insert(group);
            }
            else 
            {
                t_triangles.insert(group);
            }
            
        }
        for (int node_4b : set_2){ //2-4
            connections = { false,true,false }; //[0] we are given
            std::set<int> group = {node_1, node_2,node_3,node_4b};
            if (node_4b == node_1 || node_4b == node_3 || group.size() < 4){continue;}
           
            connections[2] = (set_3.find(node_4b) != set_3.end());  //3-4
            connections[0] = (set_1.find(node_4b) != set_1.end()); //1-4

            if (connections[0] || connections[2])
            {
                c_cycle.insert(group);
            }
            else 
            {
                t_triangles.insert(group);
            }
            
        }
        for (int node_4c : set_3){ //3-4
            connections = { false,false,true }; //[0] we are given
            std::set<int> group = {node_1, node_2,node_3,node_4c};
            if (node_4c == node_1 || node_4c == node_3 || group.size() < 4){continue;}
           
            connections[1] = (set_2.find(node_4c) != set_3.end());  //2-4
            connections[0] = (set_1.find(node_4c) != set_1.end()); //1-4

            if (connections[0] || connections[1])
            {
                c_cycle.insert(group);
            }
            else 
            {
                t_triangles.insert(group);
            }
            
        }
    }
    return cliques; //would obviously need to figure out how to return multiple (/pass them by reference)
}

//t_trianlge finder
std::set<std::set<int> >  t_trianlge (std::set<std::set<int> > triangles, 
    std::set<std::set<int> > two_stars,std::map<int, std::set<int> > &node_map )
     //6 total edges in a 4-clique
{
    std::set<std::set<int> > t_triangle;
    std::set<std::set<int> > cliques;
    //do this one for iterating over Nodes
    for (std::set<int> tri : triangles){ //for each edge
        //
        int node_1 = *tri.begin();
        int node_2 = *next(tri.begin(),1);
        int node_3 = *next(tri.begin(),2);
        
        //std::set<int> set_1 = node_map[node_1];
        //std::set<int> set_2 = node_map[node_2];
        //std::set<int> set_3 = node_map[node_3];
        
        
        for (std::set<int> star : two_stars){ 
            int node_4 = *star.begin();
            int node_5 = *next(star.begin(),1);
            int node_6 = *next(star.begin(),2);
            //here is where we should check if we need to break early
            if ((node_6 > node_3 && node_4 > node_1) ||
                (node_5 > node_2 && node_4 > node_1) ||
                (node_6 > node_2 && node_5 > node_2)) //took time from 9+ hours to 1 hr and 23 minutes, would be great if it stays this way  
                {
                    break;
                }

            //could probably optimize this
            std::vector<int> intersection;
            std::set_intersection(tri.begin(),tri.end(),star.begin(),star.end(), std::back_inserter(intersection));
            if (intersection.size() == 2) {
                std::copy(intersection.begin(), intersection.end(), std::inserter(tri, tri.end())); //adds to tri
                t_triangle.insert(tri);
                break;
            }
            
        }

    }
    std::cout<< t_triangle.size() << " t_triangles" <<std::endl;
    return t_triangle;
}
//main for testting triangles sequential (& Clique sequential)
/*

int main()
{
    std::cout<< "-------------------initializing data----------------" << std::endl;
    //intialize needed structures
    std::string file_path = "../ca-AstroPh.txt";
    std::map<int,std::set<int> > node_map;
    std::set<std::pair<int,int> > edge_set;
    //fill out edge_set and node_map
    read_edges(file_path,node_map, edge_set);
    std::cout<< edge_set.size()<< " edges" << std::endl;
    std::set<std::set<int> > triangles;
    std::set<std::set<int> > two_stars;
    std::set<std::set<int> > cliques;
    std::cout<< "-------------------Starting  Program----------------" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    //clique_four(edge_set, node_map, cliques);
    std::cout<<cliques.size()<< " cliques" <<std::endl;
    
    triangle_finder_e(edge_set, node_map,triangles, two_stars);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = duration.count() / 60;
    auto seconds = duration.count() % 60;

    std::cout<< "There are "<< triangles.size() << " triangles and " 
    << two_stars.size() << " two stars in " << file_path << "!" <<std::endl;

    std::cout<< "-------here come the stats------" <<std::endl;
    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor " << minutes<< " minutes and " << seconds << " seconds" <<std::endl;

    return 0;
} 
*/

/*
//parallel 3 case
int main(){
    std::cout<< "-------------------initializing data----------------" << std::endl;
    //intialize needed structures
    std::string file_path = "../ca-AstroPh.txt";
    //std::string file_path = "../test_data.txt";
    std::map<int,std::set<int> > node_map;
    std::set<std::pair<int,int> > edge_set;
    //fill out edge_set and node_map
    read_edges(file_path,node_map, edge_set);
    std::vector<std::pair<int,int> > edge_vec (edge_set.begin(), edge_set.end());
    //n = 12, we want 0 to have 0-3, 1 4-7, 2 8-11
    int N = edge_set.size();
    int P = 10;
    int section = N/P;
    
    
    std::cout<< N << " edges" << std::endl;
    
    //parallel 3 case
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    
    for (int i = 0; i < P; i++){
        
        std::vector<std::pair<int,int> > edge_sub;
        if (i == P-1 && (N % P != 0)){
            edge_sub.assign(edge_vec.begin() + (i * section), edge_vec.end()); //might not need the -1 if the end isn;t inclusive
        
        }
        else{
            edge_sub.assign(edge_vec.begin() + (i* section), edge_vec.begin() + 
                ((i*section) + section )); //might not need the -1 if the end isn;t inclusive
        
        }
        
        std::set<std::pair<int,int> > edge_subsection(edge_sub.begin(),edge_sub.end());

        //need to get the section size

        //don't really need to access results, as long as we can time them
        //TODO: figure out how to access results in threads

        
        auto lm = [edge_subsection, &node_map]{triangle_finder_p(edge_subsection, node_map);};
        threads.push_back(std::thread {lm});
        
    }
    // Wait for each to complete
    std::cout<<"running on " << threads.size()<< " threads" <<std::endl;
    for (std::thread& t : threads) {
        t.join();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = duration.count() / 60;
    auto seconds = duration.count() % 60;
    
    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor " << minutes<< " minutes and " << seconds << " seconds" <<std::endl;
    return 0;
}

*/
/*
//parallel clique case
int main(){
    std::cout<< "-------------------initializing data----------------" << std::endl;
    //intialize needed structures
    std::string file_path = "../ca-AstroPh.txt";
    //std::string file_path = "../test_data.txt";
    std::map<int,std::set<int> > node_map;
    std::set<std::pair<int,int> > edge_set;
    //fill out edge_set and node_map
    read_edges(file_path,node_map, edge_set);
    std::vector<std::pair<int,int> > edge_vec (edge_set.begin(), edge_set.end());
    //n = 12, we want 0 to have 0-3, 1 4-7, 2 8-11
    int N = edge_set.size();
    int P = 8;
    int section = N/P;
    
    
    std::cout<< N << " edges" << std::endl;
    
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < P; i++){
        
        std::vector<std::pair<int,int> > edge_sub;
        if (i == P-1 && (N % P != 0)){
            edge_sub.assign(edge_vec.begin() + (i * section), edge_vec.end()); //might not need the -1 if the end isn;t inclusive
        
        }
        else{
            edge_sub.assign(edge_vec.begin() + (i* section), edge_vec.begin() + 
                ((i*section) + section )); 
        
        }
        
        std::set<std::pair<int,int> > edge_subsection(edge_sub.begin(),edge_sub.end());

        //need to get the section size

        //don't really need to access results, as long as we can time them
        //TODO: figure out how to access results in threads

        
        auto lm = [edge_subsection, &node_map]{clique_four_p(edge_subsection, node_map);};
        threads.push_back(std::thread {lm});
        
    }
    // Wait for each to complete
    std::cout<<"running on " << threads.size()<< " threads" <<std::endl;
    for (std::thread& t : threads) {
        //maybe for longer running one, we print the current time each thread finishes?
        t.join();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = duration.count() / 60;
    auto seconds = duration.count() % 60;
    auto hours = minutes / 60;
    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor " << hours << " hours," <<minutes<< " minutes and " << seconds << " seconds" <<std::endl;
    return 0;
}

*/

int main(){
    std::cout<< "-------------------initializing data----------------" << std::endl;
    //intialize needed structures
    std::string file_path = "../txt/ca-AstroPh.txt";
    //std::string file_path = "../test_data.txt";

    std::map<int,std::set<int> > node_map;
    std::set<std::pair<int,int> > edge_set;
    //fill out edge_set and node_map
    read_edges(file_path,node_map, edge_set);

    std::set<std::set<int> > triangles;
    std::set<std::set<int> > two_stars;
    std::cout<< "calculating triangles" << std::endl;
    triangle_finder_e(edge_set,node_map, triangles, two_stars);
    
    std::vector<std::set<int> > triangle_vec (triangles.begin(), triangles.end());
    std::vector<std::set<int> > two_star_vec (two_stars.begin(), two_stars.end());

    //n = 12, we want 0 to have 0-3, 1 4-7, 2 8-11
    //int N = edge_set.size();
    int N = triangles.size();
    int M = two_stars.size();
    int P = 2;
    int section = N/P;
    int section_star = M/ P;
    
    std::cout<< N << " triangles and " << edge_set.size() << " edges and " << M << " two stars" << std::endl;
    
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < P; i++){
        
        std::vector<std::set<int> > edge_sub;
        std::vector<std::set<int> > edge_sub_star;
        if (i == P-1 && (N % P != 0)){
            edge_sub.assign(triangle_vec.begin() + (i * section), triangle_vec.end()); //not currently allowing each node to check all of triangle vec :(  
        
        }
        else{
            edge_sub.assign(triangle_vec.begin() + (i* section), triangle_vec.begin() + 
                ((i*section) + section )); 
        }

        if (i == P-1 && (M % P != 0)){
            edge_sub_star.assign(two_star_vec.begin() + (i * section_star), two_star_vec.end());
        
        }
        else{
            edge_sub_star.assign(two_star_vec.begin() + (i* section_star), two_star_vec.begin() + 
                ((i*section_star) + section_star )); 
        
        }
        
        std::set<std::set<int> > triangle_sub(edge_sub.begin(),edge_sub.end());
        std::set<std::set<int> > star_sub(edge_sub_star.begin(),edge_sub_star.end());

        //need to get the section size

        //don't really need to access results, as long as we can time them
        //TODO: figure out how to access results in threads

        
        //auto lm = [triangle_sub, &node_map]{clique_four_triangle(triangle_sub, node_map);}; //clique triangle ep
        //auto lm = [triangle_sub, &node_map]{ chord_cycle(triangle_sub, node_map);}; //chordal cycle (all 4)
        auto lm = [triangle_sub, star_sub,&node_map]{ t_trianlge(triangle_sub,star_sub, node_map);}; //t_triangle
        threads.push_back(std::thread {lm});
        
    }
    // Wait for each to complete
    std::cout<<"running on " << threads.size()<< " threads" <<std::endl;
    for (std::thread& t : threads) {
        //maybe for longer running one, we print the current time each thread finishes?
        t.join();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = duration.count() / 60;
    auto seconds = duration.count() % 60;
    auto hours = minutes / 60; minutes = minutes % 60;
    
    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor " << hours << " hours," <<minutes<< " minutes and " << seconds << " seconds" <<std::endl;
    return 0;
}



/*
//TODO:-> DONE create smaller subset to test this on
//fake data
    std::string test_path = "../test_data.txt";
    std::map<int,std::set<int> > node_map_t;
    std::set<std::pair<int,int> > edge_set_t;
    //fill out edge_set and node_map
    read_edges(test_path,node_map_t, edge_set_t);
    std::cout<< edge_set_t.size()<< " test edges" << std::endl;
    std::set<std::set<int> > triangles_t;
    std::set<std::set<int> > two_stars_t;
    triangle_finder_e(edge_set_t, node_map_t,triangles_t, two_stars_t);

    std::cout<< "---Start testing---- \n" << "There are "<< triangles_t.size() << " triangles and " 
    << two_stars_t.size() << " two stars in " << file_path << "! \n ----End testing----" <<std::endl;

    for (std::set<int> trip: two_stars_t){
        for (int i : trip){
            std::cout<< i <<std::endl;
        }
        std::cout<< "---" <<std::endl;
    }
*/

