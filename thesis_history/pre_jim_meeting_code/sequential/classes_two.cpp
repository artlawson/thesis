//classes
// g++ -std=c++17 classes_two.cpp
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <mutex>


//Trie object
class Trie{
    public:
        std::string name;
        std::vector<std::shared_ptr<Trie> > children;
        int n; //number of verticies in node
        std::vector<bool> adj;
        bool is_Graph;
        int num_id;
        std::vector<std::shared_ptr<Trie> > sisters; //prob don't need this anymore
};

class Vertex{
    public:
        int id;

};


class ThreadSafeUO_map {
  std::unordered_map<std::string, std::set<std::vector<int> > > map_;
  mutable std::mutex mutex_;
  
 public:
  ThreadSafeUO_map() = default;

  virtual ~ThreadSafeUO_map() { }
  void insert(const std::string &s,const std::vector<int> &item){
    std::lock_guard<std::mutex> lock(mutex_);
    map_[s].insert(item);
  }
  void print(){
      for (auto kv : map_){
        std::string key = kv.first;
        int value = kv.second.size();
        std::cout<<value <<" " <<key << "s" <<std::endl;
    }
  }
  void combine(std::map<std::string,std::set<std::vector<int> > > &local_count){
      std::lock_guard<std::mutex> lock(mutex_);
    for (auto kv : local_count){
        map_[kv.first].merge(kv.second); //merge is c++17
    }
  }

};

class WorkUnit{
public:
    std::vector<int> v_used; //
    int p_id;
    std::map<std::string, std::set<std::vector<int> > > local_GDD;
    std::shared_ptr<Trie> current_node;
    //keep unexplored verts a set bc it avoids repeats
    WorkUnit();
    WorkUnit(int dd, std::vector<int> vv_used, std::vector<int> verts_left);
};

class Graph { //currently storing globals in graph object
    public:
        int n; //number of verticies
        ThreadSafeUO_map GDD; //keep counts
        std::unordered_map<int, std::set<int> > node_map; //verify later if we need this
        std::unordered_map<int, Vertex> verticies;
        std::set<std::pair<int,int> > edge_set;
        std::vector<std::vector<bool> > adj; 
     
};


