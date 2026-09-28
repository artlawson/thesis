//classes
// g++ -std=c++17 classes_two.cpp
#include <string>
#include <chrono>
#include <ratio>
#include <utility>
#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <queue>
#include "classes_two.hh"


void ThreadSafeUO_map::insert(const std::string &s,const std::vector<int> &item){
    std::lock_guard<std::mutex> lock(mutex_);
    map_[s].insert(item);
    }
void ThreadSafeUO_map::print(){
    for (auto kv : map_){
        std::string key = kv.first;
        int value = kv.second.size();
        std::cout<<value <<" " <<key << "s" <<std::endl;
    }
}
void ThreadSafeUO_map::combine(std::map<std::string,std::set<std::vector<int> > > &local_count){
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto kv : local_count){
        map_[kv.first].merge(kv.second); //merge is c++17
    }
}


WorkUnit::WorkUnit() : WorkUnit{0, std::vector<int> (), std::vector<int>()} { }

WorkUnit::WorkUnit(int dd, std::vector<int> vv_used, std::vector<int> verts_left) :
  depth{dd},
  v_used{vv_used},
  p_id{0},
  current_node{nullptr},
  received_work_request{false}
{
    //constructor for work unit 
    //expected call from Parallel_match
    for (int i : verts_left){
        unexplored_verts[dd].insert(i);
    }
}

//empty check, if there are no verticies left to explore
//then W is considered empty and needs more work
bool WorkUnit::is_empty(){
    for (auto kv : unexplored_verts){
        if (!kv.second.empty()){
            return false;
        }
    }
    return true;
}

void WorkUnit::mini_update(std::vector<int> verts_left = std::vector<int>()){// int dd, std::vector<int> vv_used, )
    for (int i : verts_left){
        unexplored_verts[depth].insert(i); //double check here later for optimizations
                                           //when RW calls parallel_match, there's probably still values in unexplored vertices
                                           //but WAIT, that can't be an issue because RW clears each depth at the end of the loop 
                                           //but later I can/should investigate what happens if I clear unexplored_vertices b4 adding things in there
                                           //yeah this should be fine, bc when parallel_match calls this, it's at the next depth from where RW
                                           //is watching, so there shouldn't be anything to delete :)
    }
}

 void WorkUnit::set_current_node(std::shared_ptr<Trie> curr_no){
     current_node = curr_no;
     if (depth < curr_no->n){
         depth = curr_no->n;
     }
 }


//takes a work unit as an argument so we can
//just put work units in work queue
void WorkUnit::divide_work(WorkUnit* w2){ //relatively done, could  be optimized
    std::cout<<"Processor_"<<p_id<<" is sending work to Processor_"<<w2->p_id<<std::endl;
    w2->depth = depth;
    w2->explored_nodes = explored_nodes;
    if (v_used.size() > 4){std::cout<<v_used.size()<<"we getting large"<<std::endl;}
    w2->v_used.resize(v_used.size());
    w2->v_used = v_used;
    w2->current_node = current_node;
    //here is where check for "is it worth the extra headache to split and send" would go
    for (int i = 1; i<= depth; i++){ //make sure depth is 1-4
      //cut in half and resize
      int x = unexplored_verts[i].size();
	    if (x > 0){
        //puts 2nd half of w1's list into w2's
        std::set<int>::iterator it = unexplored_verts[i].begin();
        std::advance(it, (x/2));
        w2->unexplored_verts[i].insert(std::make_move_iterator(it), //Jim eying these lines
              std::make_move_iterator(unexplored_verts[i].end()));
        //removes those elements from w1's
        unexplored_verts[i].erase(it, unexplored_verts[i].end());
	    }
                
    }
    //have some call here that would signal the new data is ready and whatever barrier we used can be lifted
    w2->waiting_for_work = false; //boom^
    received_work_request = false; //work request has been fulfilled
}

 

