//algorithms 3
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> //requires c++17
#include <algorithm>
#include <map>
#include "classes_two.cpp"


std::vector<Vertex*> Matching_verticies(Trie *T,Graph &G, std::vector<Vertex*> &v_used){
    std::vector<Vertex*> cand, cond, conn, verticies;
    
    if (v_used.empty()){
        for (Vertex v : G.verticies){
            cand.push_back(&v);
        }
    }

    else{ //test this in shell
        for (Vertex* vp : v_used){
            conn.insert(conn.end(),vp->neighbors.begin(),vp->neighbors.end());

        }

        int m_val = G.verticies.size();
        Vertex *m;
        for (Vertex* vp : conn){
            if (vp->neighbors.size() < m_val){
                m = vp;
                m_val = vp->neighbors.size();
            }
        }
        bool found = false;
        int qq, tt; //compare ids
        for (Vertex* vp : m->neighbors){ //need to verify this if statement!!!!
            qq = vp -> id;
            for (Vertex* vv: v_used){
                tt = vv -> id;
                if (qq == tt){
                    found = true;
                    break;
                }
            }
            if (found){
                cand.push_back(vp);
            }
        }
    
        bool flipper;
        for (Vertex* vp : cand){
            //finish here
            flipper = false;
            for (int i = 0; v_used.size(); i++){
                //if -- checking that connections in adj match
                //expected connections of Trie
                if (T->adj[i] != G.adj[v_used[i]->id][vp->id]){ //this error is coming bc v_used[i] 

                    flipper = true;
                }
                if (flipper){break;} 
            }

            if (!flipper){
                verticies.push_back(vp);
            }
        }
    }

    return verticies;
}

void Match(Trie *T, Graph &G, std::vector<Vertex*> v_used){
    std::vector<Vertex*> V = Matching_verticies(T, G ,v_used);
    for (Vertex* lil_vert : V){
        if (T->is_Graph){ 
            //can have Trie.type = triangle, etc
            //master_counter[Trie.type]+= v_used.size();
            std::cout<< "we found one! {replace this with counter for GDD} "<< std::endl;
        }
        std::vector<Vertex*> v_children = v_used;
        v_children.push_back(lil_vert);
        for (Trie* c : T->children){
            Match(c,G,v_children);
        }
    }
}

void GtrieMatch(Trie &T, Graph &G){
    for (Trie* c : T.children){
        //uhm not  very sure about the root, might just want T.children
        std::vector<Vertex*> v_used;
        Match(c,G, v_used);
    }
}
