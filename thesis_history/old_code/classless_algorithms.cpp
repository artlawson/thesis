//algorithms without classes
//gcc -lstdc++ -std=c++11  classless_algorithms.cpp
//#include "classes.cpp"
#include <map>
#include <set>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

struct barrier { //from 361

  int howmany;
  int counter;
  //std::vector<int> counter_vec;
  std::vector<int> sample;
  std::vector<int> values;
  std::vector<int> target;
  std::map<int,std::vector<int>> samp_counts;
  std::mutex lock;
  std::condition_variable cond;

  barrier(int hm) : howmany {hm}, counter {},sample {}, values{}, target {}, lock {}, cond {} {}

};

void triangle_finder(std::vector<int> &nodes, std::vector<std::set<int> > &edges, std::map<int, std::set<int> > &node_map,
    std::set<std::set<int> > &triangles, std::set<std::set<int> > &two_stars)
{
    //do this one for iterating over Nodes
    for (int node_1 : nodes){
        //
        for (int node_2 : node_map[node_1]){

            for (int node_3 : node_map[node_2]){
                std::set<int> thrupple = {node_1,node_2,node_3}; //c++11
                std::set<int>::iterator it;
                it = node_map[node_3].find(node_1); //find node_1 in node_3 edge_list
                if (it != node_map[node_3].end()){
                    triangles.insert(thrupple);
                }
                else {two_stars.insert(thrupple);}
            }
        }
    }
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


//4 case

//4-clique (took 947 minutes [15-16 hours])

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
                    set_3b.find(node_2) != set_3b.end() ) //3-2
                {
                    cliques.insert(clique);
                }
            }
        }

    }
}

//chordal-cycle

//tailed triangle

//4-cycle

//3-star

//4-path

//currently up next:
    //parallelize the 3 case
    //get started on the 4 case
        //seems like they did a different algorithm for checking each of the 4 cases
        //and just had each run in parallel
        // I could have something that checks each in one algorithm
    //look into paper to see how they did it




int main () {
    std::vector<std::pair<int,int> > edges;
    //n = 12, we want 0 to have 0-3, 1 4-7, 2 8-11
    int N = edges.size();
    int P = 4;
    int section = N/P;
    
    //parallel 3 case
    std::vector<std::thread> threads;
    
    for (int i = 0; i < P; i++){
        std::vector<std::pair<int,int> > edge_subsection;
        if (i == P-1){
            edge_subsection.assign(edges.begin() + (i* section), edges.end()); //might not need the -1 if the end isn;t inclusive
        }
        else{
            edge_subsection.assign(edges.begin() + (i* section), edges.begin() + 
                ((i*section) + section-1 )); //might not need the -1 if the end isn;t inclusive
        }
        //need to get the section size

        //don't really need to access results, as long as we can time them
        //TODO: figure out how to access results in threads

        
        auto lm = [edge_subsection,node_map]{triangle_finder_p(edge_subsection, node_map);};
        threads.push_back(std::thread {lm});
        
    }
     // Wait for each to complete
    for (std::thread& t : threads) {
        t.join();
    }
    std::cout<< "You made it!"<< std::endl;



    return 0;

}