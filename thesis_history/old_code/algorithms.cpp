//algorithms
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
#include <map>
#include <chrono>
#include <thread>
#include <mutex>

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
    std::cout<< " clique running" << std::endl;
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
    //do this one for iterating over Nodes
    for (std::set<int> tri : triangles){ //for each edge
        //
        int node_1 = *tri.begin();
        int node_2 = *next(tri.begin(),1);
        int node_3 = *next(tri.begin(),2);
        
        std::set<int> set_1 = node_map[node_1];
        std::set<int> set_2 = node_map[node_2];
        std::set<int> set_3 = node_map[node_3];
        
        //checking for chordal cycle
        std::cout<< " chordal cycle running" << std::endl;
        for (int node_4a : set_1){ //1-4
            std::vector<bool> connections { true,false,false }; //[0] we are given
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
            std::vector<bool> connections { false,true,false }; //[0] we are given
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
            std::vector<bool> connections { false,false,true }; //[0] we are given
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
    return t_triangle;
}

//chordal cycle finder
std::set<std::set<int> >  c_cycle (std::set<std::set<int> > triangles, 
    std::set<std::set<int> > two_stars,std::map<int, std::set<int> > &node_map )
{
    std::set<std::set<int> > c_cycle;
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
        
        
        for (std::set<int> tri_2 : triangles){ 
            int node_4 = *tri_2.begin();
            int node_5 = *next(tri_2.begin(),1);
            int node_6 = *next(tri_2.begin(),2);
            //here is where we should check if we need to break early
            if ((node_6 > node_3 && node_4 > node_1) ||
                (node_5 > node_2 && node_4 > node_1) ||
                (node_6 > node_2 && node_5 > node_2)) //took time from 9+ hours to 1 hr and 23 minutes, would be great if it stays this way  
                {
                    break;
                }

            //could probably optimize this
            std::vector<int> intersection;
            std::set_intersection(tri.begin(),tri.end(),tri_2.begin(),tri_2.end(), std::back_inserter(intersection));
            if (intersection.size() == 2) {
                std::copy(intersection.begin(), intersection.end(), std::inserter(tri, tri.end())); //adds to tri
                c_cycle.insert(tri);
                break;
            }
            
        }

    }
    return c_cycle;
}