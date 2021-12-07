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


bool check_message_threshold(Graph &G){
    //will have to get current_time
    //is it time for
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - G.LAST_CHECK);
    bool is_it_time = duration.count()  > G.TIME_THRESHOLD; //will also have to clean this to be able to repeat (mod)
    //should make sure Last check is properly updated!
    if (is_it_time){
        G.LAST_CHECK = now;
    }
    return is_it_time;
}

void check_messages(WorkUnit* W, Graph &G){
    //updates the queue of work_requests
    //still not sold that we need to have this AND Receive Work
    W->received_work_request = !G.WORK_REQUESTS.empty();
    if (W->received_work_request){
        std::cout<< "There's a work request waiting on me to fulfill!"<< std::endl;
    }
    //std::cout<< "checking messages"<<std::endl;
    
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
    //std::cout<< "Match called on "<<W.current_node->name <<std::endl;
    if (check_message_threshold(G)){
        check_messages(&W,G);//check messages takes a ptr
    }
    std::vector<int> V = parallel_Matching_verticies(p_id,W.current_node,G,W.v_used);
    for (int v = 0; v < V.size() ; v++){
        W.v_used.push_back(V[v]);

        if (W.received_work_request){
            //construct work unit
            std::vector<int> unexplored_verticies = {V.begin() + v+1, V.end()}; //plus one bc we already have v in v_used
            W.mini_update(unexplored_verticies);
            
        }

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
            if (W.received_work_request){
                if (W.unexplored_verts.empty()){//if work unit hasn't been updated
                    //then construct new work unit
                    std::vector<int> unexplored_verticies = {V.begin() + v+1, V.end()};
                    W.mini_update(unexplored_verticies);
                    //i think it's pretty smart to update this way
                    //because it allows the each level to update the respective depth
                    //as it climbs back up
                }
                for (int q = j; q < W.current_node->children.size(); q++){
                    c = W.current_node->children[q];
                    W.explored_nodes.push_back(c);
                }
                return; 
                //not really sure how I'm going to terminate all of the recursive calls?
                //maybe if I keep passing work_unit?
                //mini_update should fix this?
            }
            W.set_current_node(c);
            parallel_match(p_id,G, W);
            W.set_current_node(current_node_reset);
        }
        W.v_used.pop_back();
    }

}

//current root of the problemz
void resume_work(WorkUnit &W, Graph &G){

    //when a work request comes in, we need to be able to get back to the trie object we started with
    std::shared_ptr<Trie> original_trie = W.current_node; 
    for (int q = 0; q < W.explored_nodes.size(); q++){ //W.explored has all of the parent's children
        std::shared_ptr<Trie> trie = W.explored_nodes[q];
        //these are the calculations that need to go on from cutoff of p_match
        if (W.received_work_request){
            //erase ones we've already computed
            //how/where else do these get popped if needed?????
            W.explored_nodes.erase(W.explored_nodes.begin(), W.explored_nodes.begin() + q); //Jim: This could take time
            break;
        }
        W.set_current_node(trie);
        parallel_match(-1,G,W); 
        
        //if we convert to system with T being attached to W, how would we resolve issue of parallel match
        //being terminated early because of 
    }
    //std::cout<< "made it past 1st match call in resume_work"<<std::endl;
    W.set_current_node(original_trie);
    for (int i = W.depth; i > 0; i--){
        
        for (int j = 0 ; j < W.unexplored_verts[i].size(); j++){
            if (W.received_work_request){
                //std::cout<< "work requested"<<std::endl;
                std::set<int>::iterator it;
                //must do this because you can't use addition on set iterator, see advance call below
                it = W.unexplored_verts[i].begin();
                
                if (j != 0){
                    std::advance(it, j-1);
                    W.unexplored_verts[i].erase(W.unexplored_verts[i].begin(),it);
                }
                
                break; //Jim's nervous, look into breaking out of outter loop
            }

            W.v_used.pop_back(); //first time through, this removes the vertex we left off on
            std::set<int>::iterator it = W.unexplored_verts[i].begin();
            std::advance(it, j);
            W.v_used.push_back(*it);
            if (W.current_node->is_Graph){ //<- be cautious of this one
                
                std::vector<int> match_vec = W.v_used;
                std::sort(match_vec.begin(), match_vec.end());
                W.local_GDD[W.current_node->name].insert(W.v_used);
                //G.GDD.insert(W.current_node->name, match_vec );
            }
            
            for (int q = 0; q < W.explored_nodes.size(); q++){ //W.explored has all of the parent's children
                std::shared_ptr<Trie> child = W.explored_nodes[q];
                if (W.received_work_request){
                    //stop and construct work array
                    //W.update;
                    W.set_current_node(original_trie);
                    W.explored_nodes.erase(W.explored_nodes.begin(), W.explored_nodes.begin() + q);
                    break;
                }
                //std::cout<<"bug hunting"<<std::endl;
                W.set_current_node(child);
                parallel_match(-1,G,W);
                //std::cout<< "made it past 2nd match call in resume_work"<<std::endl;
            }
            
            W.set_current_node(original_trie);
        }
        W.v_used.pop_back(); //this is because it is going back recursively?
        W.unexplored_verts[i].clear(); //no longer unexplored if we've explored them :)
        
        //will need to investigate if we need this last break when we are stopping for work request
        //i don't think we would???
        //if (recieve_work_request){break;}  
    }

}



//instead of calling in higher function,we can call this in parallel match
//and let that force it to exit
//can also have this be the thing that constructs the work unit? that might get messy with recursion tho
void recursive_process(WorkUnit* W, Graph &G){ 
    //this function does the checks to see if there are any work messages
    if (check_message_threshold(G)){
        check_messages(W,G);
        //G.TIME_THRESHOLD*=1.2; //maybe ask Jim a better way to do this
    }
    //else{ G.TIME_THRESHOLD*= (1/3);}
    
    if (W->received_work_request){
        //return so parallel_count can split work
        return;
    }
    
    else{
        if (W->is_empty()){
            return; //return so work can be requested
        }
        else {
            //std::cout<<"resuming my work :)" <<std::endl;
            resume_work(*W, G);
        }
    }
}


void parallel_count(int p_id, WorkUnit &W, std::shared_ptr<Trie> T, Graph &G){
    W.p_id = p_id;
    W.set_current_node(T);
    parallel_match(p_id, G, W); //W = initial_work; //call here to Parallel Match
    //std::cout<<"processor: "<< p_id << " paused parallel_match" <<std::endl;
    while (!W.all_done){ 
        recursive_process(&W, G);
        if (W.received_work_request){ 
            WorkUnit* w2 = G.WORK_REQUESTS.pop();
            if (w2){ //this might not be necessary, but currently keeping here in case of race condition
                W.divide_work(w2);
            }    
        }
        if (W.is_empty()){ //check if W is empty
            if (G.WORK_REQUESTS.size() != G.P - 1){ //if there is any more work to be had
                //std::cout<< "processor " << p_id << " is asking for more work"<<std::endl;
                ask_for_more_work(&W, G);//ask takes a pointer to W
                //std::cout<< "processor " << p_id << " got EXACTLY what he was looking for ;)"<<std::endl;
                //std::cout<< "Are we done? 0 for no, 1 for yes: "<< W.all_done<<std::endl;
                if (W.all_done){
                    //std::cout<<"about to return from requesting more work"<<std::endl;
                    return;
                }
            }
            else{ 
                //this means we have reached the final processor looking for work.
                //std::cout<< "processor " << p_id << " is the last one standing"<<std::endl;
                while (!G.WORK_REQUESTS.empty()){
                    WorkUnit* wx = G.WORK_REQUESTS.pop();
                    wx->waiting_for_work = false; //end "waiting for work"
                    wx->all_done = true; //break work loop
                }
                W.all_done = true;
                return; //this shouldn't be necessary
            }
        }
        
    }
}
