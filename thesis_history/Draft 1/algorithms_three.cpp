//algorithms 3
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> //requires c++17
#include <algorithm>
#include <map>
#include "classes_two.cpp"

int edj = 0;

std::vector<int> Matching_verticies(Trie *T,Graph &G, std::vector<int> &v_used){
    std::vector<int> cand, cond, conn, verticies;
    //std::cout<< "match was called"<<std::endl;
    
    if (v_used.empty()){
        for (int v = 0; v < G.n; v++){
            cand.push_back(v);
        }
    }

    else{ //test this in shell
        for (int vp : v_used){
            
            conn.insert(conn.end(),G.node_map[vp].begin(),G.node_map[vp].end());
        }
        
        if (conn.size() == 0){
            std::cout << "another empty" << std::endl;
            return verticies; //returns empty vect
        }

        int m_val = G.verticies.size();
        int m;
        

        for (int vp : conn){
            if (G.node_map[vp].size() < m_val){
                m = vp;
                m_val = G.node_map[vp].size();
            }
        }
        
        // bool found = false;
        for (int vp : G.node_map[m]){ //need to verify this if statement!!!!
    
            if (std::find(v_used.begin(),v_used.end(),vp) != v_used.end()){
                cand.push_back(vp);
            }
    
        }
        
    }
    
    bool flipper;
    for (int vp : cand){
        //finish here
        flipper = false;
        for (int i = 0; v_used.size(); i++){
            //if -- checking that connections in adj match
            //expected connections of Trie
            if (T->adj[i] != G.adj[v_used[i]][vp]){  

                flipper = true;
            }
            if (flipper){break;} 
        }

        if (!flipper){
            if (T->n == 1){
                edj++;
            }
            verticies.push_back(vp);
        }
    }
    
    return verticies;
}

void Match(Trie *T, Graph &G, std::vector<int> v_used){
    std::vector<int> V = Matching_verticies(T, G ,v_used); //Matching_vert returning empty array for all edges
    //std::cout<< T->n<<std::endl;// the fact we are only getting two here says we aren't getting to triangle case
           
        if (T->is_Graph){ 
            //can have Trie.type = triangle, etc
            //master_counter[Trie.type]+= v_used.size();
            std::cout<< "we found one! {replace this with counter for GDD} "<< std::endl;
        }
        std::vector<int> v_children = v_used;
        
        v_children.push_back(lil_vert);
        if (T->n == 2){
                std::cout<<"before match call with this many children -> " << T->children.size() <<std::endl;
            }
        for (Trie* c : T->children){
            Match(c,G,v_children); //bad alloc here
        }
    }
        
        
}

void GtrieMatch(Trie &T, Graph &G){
    for (Trie* c : T.children){
        std::vector<int> v_used;
        Match(c,G, v_used);
    }
    std::cout<< "edj"<< edj <<std::endl;
}
