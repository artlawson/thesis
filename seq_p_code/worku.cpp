//work_unit w/ work sharing
#include <string>
#include <filesystem> //requires c++17
#include <algorithm>
#include "classes_two.hh"
#include "file_reader_two.hh"


//WorkUnit Class approach
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

void parallel_candidates_creator(int p_id,std::vector<int> &cand, std::vector<int> &v_used, Graph &G){
    if (v_used.empty()){
        //for loop should be fine for P = 1 bc verticies are 0 index
        for (int v = p_id; v < G.v; v+= G.P){ //divides 'round-robin' style
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

std::vector<int> parallel_Matching_verticies(int p_id, std::shared_ptr<Trie> T,Graph &G, std::vector<int> &v_used){
    std::vector<int> cand, verticies;
    //populate candidates
    parallel_candidates_creator(p_id,cand,v_used, G);
    //check candidates connections to current path in v_used

    for (int vp : cand){
        if (check_connections(vp, v_used, G,T)){
            verticies.push_back(vp);
        }
    }
    
    return verticies;
}


void ask_for_more_work(WorkUnit* W, Graph &G){
    //send message "hey I need work" Global queue
    //have one processor handling requests?
    W->waiting_for_work = true;
    G.WORK_REQUESTS.push(W);
    while (true){//loop until it gets the work it needs
        if (!W->waiting_for_work){
            break;
        }
    }
    //std::cout<< "Thank you for saving me from unemployment "<< W->is_empty()<<std::endl;

    //could put barrier here that stops computation until the work has been sent back
    //but while loop will surfice for now
    
    return;
}



void parallel_match(int p_id, Graph &G, WorkUnit &W){
    std::vector<int> V = parallel_Matching_verticies(p_id,W.current_node,G,W.v_used);
    for (int v = 0; v < V.size() ; v++){
        W.v_used.push_back(V[v]);

        if (W.current_node->is_Graph){ 
            std::vector<int> match_vec = W.v_used;
            std::sort(match_vec.begin(), match_vec.end()); //sort to avoid repeats in GDD (1,2,3) = (2,3,1)
            //print_vector(W.v_used);
            W.local_GDD[W.current_node->name].insert(match_vec);
            //G.GDD.insert(W.current_node->name, match_vec );
        }
        std::shared_ptr<Trie> c, current_node_reset;
        current_node_reset = W.current_node; //reset current node for each iteration of the loop
        for (int j = 0; j < W.current_node->children.size() ; j++){
            c = W.current_node->children[j];
            W.set_current_node(c);
            parallel_match(p_id,G, W);
            W.set_current_node(current_node_reset);
        }
        W.v_used.pop_back();
    }

}

void parallel_count(int p_id, WorkUnit &W, std::shared_ptr<Trie> T, Graph &G){
    W.p_id = p_id;
    W.set_current_node(T);
    parallel_match(p_id, G, W);
    std::cout<<"Processor: " << p_id <<" is done!";
}
