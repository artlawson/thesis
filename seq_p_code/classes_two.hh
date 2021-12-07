#ifndef __CLASSES_TWO_
#define __CLASSES_TWO_

#include <string>
#include <chrono>
#include <ratio>
#include <utility>
#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <map>
#include <queue>
#include <mutex>

#include <iostream>


	
template<typename T>
class ThreadsafeQueue {
  std::queue<T> queue_;
  mutable std::mutex mutex_;

 public:
  ThreadsafeQueue() = default;
  ThreadsafeQueue(const ThreadsafeQueue<T> &) = delete ;
  ThreadsafeQueue& operator=(const ThreadsafeQueue<T> &) = delete ;
 
  ThreadsafeQueue(ThreadsafeQueue<T>&& other) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_ = std::move(other.queue_);
  }

  bool empty() const {
    return queue_.empty();
  }
 
  virtual ~ThreadsafeQueue() { }
 
  unsigned long size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }
 
  T pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return {};
    }
    T tmp = queue_.front();
    queue_.pop();
    return tmp;
  }
 
  void push(const T &item) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(item);
  }
};


class ThreadSafeUO_map {
  std::unordered_map<std::string, std::set<std::vector<int> > > map_;
  mutable std::mutex mutex_;
  
 public:
  ThreadSafeUO_map() = default;

  virtual ~ThreadSafeUO_map() { }
  void insert(const std::string &s,const std::vector<int> &item);
  void print();
  void combine(std::map<std::string,std::set<std::vector<int> > > &local_count);

};


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

class WorkUnit{
public:
    int depth; //D
    std::vector<int> v_used; //
    bool waiting_for_work;
    int p_id;
    bool all_done;
    std::map<std::string, std::set<std::vector<int> > > local_GDD;
    std::shared_ptr<Trie> current_node;
    bool received_work_request;
    std::vector<std::shared_ptr<Trie> > explored_nodes; //N (nodes)
    //keep unexplored verts a set bc it avoids repeats
    std::unordered_map<int, std::set<int> > unexplored_verts; //Ui (verticies) maps depth to unexplored verticies
    WorkUnit();
    WorkUnit(int dd, std::vector<int> vv_used, std::vector<int> verts_left);
    bool is_empty();
    void mini_update(std::vector<int> verts_left);
    void divide_work(WorkUnit* w2);
    void set_current_node(std::shared_ptr<Trie> curr_no);
};

class Vertex{
    public:
        int id;
};

class Graph { //currently storing globals in graph object
    public:
        int v; //number of verticies
        int P; //number of processors
        int e; //number of edges
        int TIME_THRESHOLD;
        bool FINISHED;
        //std::unordered_map<std::string, std::set<std::vector<int> > > GDD; //keep counts
       ThreadSafeUO_map GDD;
        ThreadsafeQueue<WorkUnit*> WORK_REQUESTS;
        std::unordered_map<int, std::set<int> > node_map; //verify later if we need this
        std::unordered_map<int, Vertex> verticies;
        std::set<std::pair<int,int> > edge_set;
        std::vector<std::vector<bool> > adj;   
};

#endif
