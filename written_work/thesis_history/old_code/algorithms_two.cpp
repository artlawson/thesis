//algorithms 2
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
#include "classes_two.cpp"

//functionMATCHINGVERTICES(T,Vused)
std::vector<Vertex> MATCHING_VETRTICIES(Trie &T,Vertex &V, Graph &G){
    //if Vused = ∅ then Vcand := V (G)
    if (V.used.empty()){
        V.cand = G.verticies; //presumably, the vertex set of G
    }
    //else
    else{
        //Vconn ={v:v=Vused[i],T.in[i]∨T.out[i]} m := m ∈ Vconn : ∀v∈ Vconn,|N(m)| ≤ |N(v)| Vcand :={v∈N(m):v̸∈Vused}
        for (int i = 0; i < V.used.size(); i++){
            //uhm double check this.
            if (T.in[i] || T.out[i]){ //already matched verticies that are already connected to the new vertex
                V.conn.push_back(V.used[i]);
            }
        }
        Vertex m;
        float m_neighbors = 'inf';
        for (Vertex v : V.neighbors){
            if (m_neighbors == 0){ break;}

            if (v.neighbors.size() < m_neighbors){
                m = v;
                m_neighbors = v.neighbors.size();
            }
        }

        for (Vertex v : m.neighbors){ //n = neighbors!
            if (std::find(V.used.begin(),V.used.end(),v) == V.used.end()){
            //if (v not in V.used){
                V.cand.push_back(v);
            }
        }
    }
    //Vertices = ∅ forallv∈Vcanddo
    std::vector<Vertex> Verticies;
    for (Vertex v : V.conn){
        //if ∀i∈[1..|Vused|]:
        for (int i = 0; i < V.used.size(); i++){
            //T.in[i] = GAdj[Vused[i]][v]∧ T.out[i] = GAdj[v][Vused[i]] then
            if ((T.in[i] == G.adj[V.used[i].id][v.id]) ||
                (T.out[i] == G.adj[v.id][V.used[i].id])){

                //if ∃ C ∈ T.cond : Vused + v respects symetry breaking conditions then 
                std::vector<Vertex> combo = V.used;
                combo.push_back(v);
                if (T.cond(combo)){
                    //Vertices = Vertices ∪ {v}
                    Verticies.push_back(v);
                }
            }
        }
          
    }
    return Verticies;
}

//procedure MATCH(T, Vused)
void MATCH(Trie T, std::vector<Vertex> Vused){

    //V = MATCHINGVERTICES(T, Vused) forallnode v ofV do
    std::vector<Vertex> V = MATCHING_VETRTICIES(T,Vused);
    for (Vertex v : V){
        T.add_vertex(v);
        //if T.isGraph then FOUNDMATCH() 

        //WE CAN LEAVE THIS AS IS NOW
        //BUT EVENTUALLY WE'LL WANT TO CHANGE IT S.T.
        //INSTEAD OF CHECKING IF T IS A GRAPH, WE CHECK WHAT KIND IT IS?
        if (T.is_Graph){ //this probably means that each v in V is supposed to create a new version of T
            //cout Vused + v
            return;
            //return FOUND_MATCH(); //this means we have found an occurance of that subgraph
        }
        //for all children c of T do
        std::vector<Vertex> temp_used;
        for (Trie c : T.children){
            //MATCH(c, Vused ∪nion {v}) 
            temp_used = Vused;
            temp_used.push_back(v);
            MATCH(c,temp_used);
        }
        T.remove_vertex(v);
    }
}

//procedure MATCHALL(T, G)
void match_all(Trie &T, Graph &G) {
 //&node_map, std::set<std::pair<int,int> > &edges){
//for all children c of T.root do
    for (Trie c : T.root.children){
        //MATCH(c, ∅) 
        std::vector<Vertex> VUsed;
        MATCH(c, VUsed);
    }
 }



//still need to figure out:
// what is N(m)/ N(v)

//function GTCanon(G)
void GTCanon(Graph &G){
    //G := nautyLabeling(G) 
    //G = nautyLabeling(G); 
    //for all i ∈ V (G) do
    for (int i : G.verticies){
        //current degree[i] := nr ingoing+outgoing connections of i
        G.current_degree[i] = G.node_map[G.verticies[i]].size();
        //global degree[i] := last degree[i] := current degree[i] 
        G.global_degree[i] = G.current_degree[i]; G.last_degree[i] = G.current_degree[i];
    }

    //for pos : |V(G)| down to 1 do
    for (int pos = G.verticies.size() - 1; pos > -1; pos--){
        //Choose umin subject to:
        Vertex umin;
        int umin_degree = G.verticies.size() + 318;

        for (Vertex i : G.verticies){
            //• umin is still not labeled and is not an articulation point
            //can check articulation by making sure that each node it is connected to 
            //doesn't have that as it's only connection
            if ((std::find(G.articluation_points.begin(),G.articluation_points.end(), i) != 
                G.articluation_points.end()) &&
                G.label.count(i.id) > 0 ) {//count returns 0 if key not in map
                    //• umin has minimum current degree
                    if (G.current_degree[i.id] < umin_degree){
                        umin = i; umin_degree = G.current_degree[i.id];
                    }
                    //• In case of equal min. current degree, choose umin with min. last degree
                    else if (G.current_degree[i.id] == umin_degree){

                        if (G.last_degree[i.id] < G.last_degree[umin.id]){
                            umin = i; umin_degree = G.current_degree[i.id];
                        }
                        //• In case of equal min. last degree, choose umin with min. global degree
                        else if (G.last_degree[i.id] == G.last_degree[umin.id]){
                            if (G.global_degree[i.id] < G.global_degree[umin.id]){
                            umin = i; umin_degree = G.current_degree[i.id];
                            }     
                        }
                    }
                }
            else{continue;}

        }
            
        //labelCanonG[umin] := pos
        G.label[umin.id] = pos;
        //last degree[] := current degree[]
        G.last_degree = G.current_degree;
        //update current degree[] removing umin connections
        G.remove_vertex(umin);
        
    }
    //return CanonG
    std::cout<< "We made it through the fire"<<std::endl;
}

//procedure insertRecursive(M,T,k) 
void insertRecursive(std::vector<std::vector<int> > &M, Trie &T, int k) {
    //M must be the adjacency matrix for that specific Graph shape ****
    //if k = numberRows(M ) then
    if (k == M.size()){ 
        //c.isGraph = true
        T.is_Graph = true; //really banking on this ^ being a typo :(
    }
     //else
    else{
        //for all children c of T do
        std::vector<int> subsection_in;
        std::vector<int> subsection_out;
        for (int i = 0; i < k + 1; i++){
            subsection_out.push_back(M[k][i]); //will probably want to make this more efficient
            subsection_in.push_back(M[i][k]); //might also have these backwards

        } 
        for (Trie c : T.children){
            //if (c.out = first k + 1 values of k-row of M) AND
            //(c.in = first k + 1 values of k-column of M)
            if ((c.out == subsection_out) &&
                (c.in == subsection_in)) {
                //then insertRecursive(M, c, k + 1)
                insertRecursive(M,c,k+1);
                return;  
            }
            
        }
        //nc := new g-trie node
        Trie nc;
        //nc.in := first k + 1 values of k-row of M nc.out := first k + 1 values of k-column of M nc.isGraph := false
        nc.in = subsection_out; nc.out = subsection_in;
        //T .insertChild(nc)
        T.children.push_back(nc);
        //insertRecursive(M, nc, k + 1)
        insertRecursive(M,nc,k + 1);  
    }
    
    
}

//procedure gtrieInsert(G,T)
void gtrieInsert(Graph &G, Trie &T){
    //M := adjacency matrix of GTCanon(G) 
    GTCanon(G); //may just want to make GTCanon return a new graph
    //insertRecursive(M,T,0)
    insertRecursive(G.adj, T, 0);
    
}

//after gtriesInsert, we have a g-trie of G
//now we need to make g-trie work with sequential algorithm


