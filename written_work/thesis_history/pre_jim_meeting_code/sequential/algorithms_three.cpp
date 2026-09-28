//algorithms 3
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> //requires c++17
#include <algorithm>
#include <map>
#include <queue>
#include <tuple>
//#include "p_algorithms.cpp"
//#include "classes_two.cpp"
//#include "worku.cpp"

void candidates_creator(std::vector<int> &cand, std::vector<int> &v_used, Graph &G){
    if (v_used.empty()){
        for (int v = 0; v < G.n; v++){
            cand.push_back(v);
        }
        return;
    }
    else{
       //this is going to be a bottleneck for larger code, but it is pretty easy to see
       //it's correct
       std::vector<int> conn;
       for (int vp : v_used){
            //conn = neighbors of all verticies in v_used
            conn.insert(conn.end(),G.node_map[vp].begin(),G.node_map[vp].end());         
        }
        
       for (int vp : conn){
           if (std::find(v_used.begin(),v_used.end(), vp) == v_used.end() &&
                    std::find(cand.begin(),cand.end(), vp) == cand.end()){ //this line might be redundant, should check later
                cand.push_back(vp);
            }
            
           for (int neighbor : G.node_map[vp]){
               if (std::find(v_used.begin(),v_used.end(), neighbor) == v_used.end() &&
                    std::find(cand.begin(),cand.end(), neighbor) == cand.end()){   
                    cand.push_back(neighbor);
               }
           }
        }  
    }
}



bool check_connections(int vp, std::vector<int> &v_used, Graph &G, std::shared_ptr<Trie> T){
    //finish here
    for (int i = 0; i < v_used.size(); i++){
        //checking that connections in adj match expected connections of Trie
        if (T->adj[i] !=G.adj[v_used[i]][vp]){ //this needs to be tightened up
            return false;
        }
    }
    return true;
}


std::vector<int> Matching_verticies(std::shared_ptr<Trie> T,Graph &G, std::vector<int> &v_used){
    std::vector<int> cand, verticies;
    //populate candidates
    candidates_creator(cand,v_used, G);
    //check candidates connections to current path in v_used

    for (int vp : cand){
        if (check_connections(vp, v_used, G,T)){
            verticies.push_back(vp);
        }
    }
    
    return verticies;
}


void Match(std::shared_ptr<Trie> T, Graph &G, std::vector<int> v_used,
    std::unordered_map<std::string, std::set<std::vector<int> > > &GDD){
    std::vector<int> V = Matching_verticies(T, G ,v_used);
    
    //currently, V is 0 for all of size two, hmmm
    for (int lil_vert : V){
        v_used.push_back(lil_vert);
        if (T->is_Graph){ 
            //can have Trie.type = triangle, etc
            GDD[T->name].insert(v_used);
        }
        for (std::shared_ptr<Trie> c : T->children){
            
            Match(c,G,v_used, GDD);
        }

        v_used.pop_back();
    }     
}

void GtrieMatch(std::shared_ptr<Trie> T, Graph &G,
    std::unordered_map<std::string, std::set<std::vector<int> > > &GDD ){
    //currently not getting children for node case to node ca
    for (std::shared_ptr<Trie> c : T->children){
        std::vector<int> v_used;
        Match(c,G, v_used,GDD);
    }
    
}