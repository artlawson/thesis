//algorithms 3
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> //requires c++17
#include <algorithm>
#include <map>
#include <queue>
#include <tuple>
#include "classes_two.cpp"
#include "worku.cpp"




/*

void parallel_match(std::shared_ptr<Trie> T, Graph &G, std::vector<int> v_used,
    std::unordered_map<std::string, std::set<std::vector<int> > > &GDD
    std::vector<int> &work_unit){
    //
    if (check_message_threshold()){
        check_messages();
    }
    std::vector<int> V = Matching_verticies(T,G,v_used);
    for (int v = 0; i < V.size() : v++){
        v_used.push_back(V[v]);

        if (recieved_work_request()){
            //request to divide work
            v_used.pop_back(); //remove extra element we just put at the end
            work_unit.push_back(v_used.size()); //size of explored verticies thus far (depth)
            for (int x : v_used){
                work_unit.push_back(x);
            }

            //pushed V's into work_unit^
            int q = T->children.size();
            work_unit.push_back(q);// watch out for the sizing on this
            for (std::shared_ptr<Trie> c : T->children){
                work_unit.push_back(c->num);
            }
            
            work_unit.push_back(V.size() - v + 1);//size of unexplored verticies
            //need to add explored and unexplored verticies
            for (int a = v; a < V.size(); a++){
                work_unit.push_back(V[a]);
            }

            return;
        }
        int x = work_unit.size();
        if (T->is_Graph){ 
            GDD[T->name].insert(v_used);
        }
        //recursive call
        for (std::shared_ptr<Trie> c : T->children){
            
            parallel_match(c,G, v_used, GDD, state);
        }
        v_used.pop_back();
    }

}







void resume_work(WorkUnit W){
    //WorkUnit Class approach

    for (int i = W.D-1; i > -1; i--){ //double check the i > 0
        //trie map =   {trie_num -> trie*}
        std::vector<std::shared_ptr<Trie> n_remaining = W.explored_nodes; //past D + past V + Ni  ////might need to check this //def need to fix this, incorporating the N from parallel match
        //next add sibilings to n_remaining
        //we'll see soon how necessary this is
        n_remaining.insert(n_remaining.end(), n_remaining[0]->sisters.begin(), n_remaining[0]->sisters.begin());

        for (std::shared_ptr<Trie> trie : n_remaining){
            //these are the calculations that need to go on
            if (recieved_work_request()){
                //stop and construct work array
                w.update; //might need to add arguments to update to check which state we got stopped at
                break;
            }
            parallel_match(trie, W.v_used, GDD);
        }
    
        for (int j = 0 ; j < W.unexplored_verts.size(); j++){
            if (recieved_work_request()){
                //stop and construct work array
                //the work array communicates whether or not we were stopped mid-search
                W.update;
                break;
            }

            W.v_used.pop_back; //first time through, this removes the vertex we left off on
            W.v_used.insert(W.unexplored_verts[j]);
            if (W.explored_nodes[j].is_Graph()){
                GDD[T->name].insert(W.v_used);;
            }

            for (std::shared_ptr<Trie> child : n_remaining){ //n_remaining has all of the parent's children
                if (recieved_work_request){
                    //stop and construct work array
                    W.update;
                    break;
                }
                parallel_match(child, W.v_used);
            }
        }
        W.v_used.pop_back(); //this is because it is going back recursively?

    }

}


void parallel_count(){
    W = initial_work; //call here to Parallel Match? 
    while (not_finished){
        recursive_process(&W);
        if recieved_work_request(){
            w2 = dividework(&w1);
            send_work(w2);

        }
        if (W.size() == 0){
            W = ask_for_more_work();
        }
    Aggregate_results();
    }
}



std::vector<int> divide_work(std::vector<int> W){ //relatively done, could  be optimized
    //returns two vectors, 
    //could later modify (if it makes sense)
        // s.t. it modifies vector 1 and returns vector 2
    //also have threshold for when array is too small to split
    //
    int D = w[0];
    std::vector<int> w1,w2 = {W.begin(), W.begin() + D}; //+ D bc 0 index {copies D, and V's}
    //now we need to 
        //split unexplored verticies
        //fix values for both arrays (U value counts)


    //initialize x at position of first U
    int x = (2 *D) +1;
    while (x < W.size()){
        int U = W[x]; //initialized as U1
        //need to store location of U so we can go back and put the |number| there
         int one = 0; int one_place = w1.size();
         int two = 0; int two_place = w2.size();
        x++; //move from U to first element at that depth
        for (int i = 0; i < U; i++){
            if (W[x] % 2 == 1){
                w2.push_back(W[x]);
                two++;
                //W.erase(W.begin() + x); //removes odd element from w1
            }
            else{
                w1.push_back(W[x]); //need to figure out later how to 
                one++;
            }
            x++; //increment x at end of loop
            //so it should be at next U at end of loop
        }
        w1[one_place] = one; //this keeps an accurate count for the U's
        w2[two_place] = two;
        
    }
    return w2;
}

bool recieve_work_request(){
    //checks queue of work_requests
    //if there's something there, 
    //we update W in recursive process
    //and here we just return true
    return !WORK_REQUESTS.empty();
}

bool check_message_threshold(){
    //will have to get current_time
    //is it time for 
    return current_time > message_threshold; //will also have to clean this to be able to repeat
}

void check_messages(){
    //updates the queue of work_requests
    
}

void send_work(std::vector<int> W){
    return;
}

std::vector<int> ask_for_more_work(){
    //send message "hey I need work"
    
    //need to figure out global structure for messages
    //have one processor handling requests?
    std::vector<int> new_work;
    
    
    return new_work;
}

void recursive_process(std::vector<int> W){
    //this function does the checks to see if there are any work messages
    if (check_message_threshold()){
        check_messages();
    }
    if (recieve_work_request()){
        //stop and construct work array


    }
    else{
        //pass because there's no need to check anything 
        return;
    }
}





void resume_work_int(std::vector<int> W){
    //integer approach to work array
    int D = W.d;
    std::vector<int> v_used = {W.begin() + 1, W.begin() + D + 1}; //2nd element through 2nd + D
    std::vector<int> work_unit;
    //{big_vector.begin() + 3, big_vector.end() - 2};


    for (int i = D-1; i > -1; i--){ //double check the i > 0
        // D [V*D] [N*D] [U1] [u * U1] [UD] [u * UD]

        //could get creative with n_remaining
        //by keeping track of siblings with node
        
        //trie map =   {trie_num -> trie*}
        std::vector<std::shared_ptr<Trie> n_remaining = trie_map[W[D+1 + i]]; //past D + past V + Ni  ////might need to check this //def need to fix this, incorporating the N from parallel match
        //next add sibilings to n_remaining
        n_remaining.insert(n_remaining.end(), n_remaining[0]->sisters.begin(), n_remaining[0]->sisters.begin());

        for (std::shared_ptr<Trie> trie : n_remaining){
            //these are the calculations that need to go on
            if (recieved_work_request()){
                //stop and construct work array
                break;
            }
            parallel_match(trie, v_used, GDD);
        }
        std::vector<int> unexplored = W{Ui.begin(), Ui.end()};
        for (int j = 0 ; j < unexplored.size(); j++){
            if (recieved_work_request()){
                //stop and construct work array
                //the work array communicates whether or not we were stopped mid-search
                break;
            }

            v_used.pop_back; //first time through, this removes the vertex we left off on
            v_used.insert(unexplored[j]);
            if (.is_Graph()){
                found_match;
            }

            for (std::shared_ptr<Trie> child : n_remaining){ //n_remaining has all of the parent's children
                if (recieved_work_request){
                    //stop and construct work array
                    break;
                }
                parallel_match(child, v_used);
            }
        }
        v_used.pop_back(); //this is because it is going back recursively?

    }

}



        cand creator else
        std::vector<int> conn;
        for (int vp : v_used){
            //conn = neighbors of all verticies in v_used
            conn.insert(conn.end(),G.node_map[vp].begin(),G.node_map[vp].end());
        }
        
        int m_val = G.verticies.size();
        int m;
        currently, this is over-optimizing
        and we aren't getting through all realistic possibilities

        For now, we can add all neighbors of vp in conn, and let 
        check_connections pick out valid ones

        later on we can implement symetry conditions & optimize candidate_creator
        to better limit search without compromising
        for (int vp : conn){
            if (G.node_map[vp].size() < m_val){
                m = vp;
                m_val = G.node_map[vp].size();
            }
            else if (G.node_map[vp].size() == m_val){
                if (std::find(v_used.begin(),v_used.end(),vp) == v_used.end()){
                    m = vp;
                    m_val = G.node_map[vp].size();

                }
            }
        }
        
        //in two_star case, 0-1 and 0-2
        //0= m, and 0's only neighbor is 1,
        //when v_used = {1}, 0 wasn't getting added, and neither was one
        if (std::find(v_used.begin(),v_used.end(),m) == v_used.end()){
                cand.push_back(m);
            }
        for (int vp : G.node_map[m]){
    
            if (std::find(v_used.begin(),v_used.end(),vp) == v_used.end()){
                cand.push_back(vp);
            }
    
        }  
        */