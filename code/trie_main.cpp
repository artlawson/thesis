//trie_main
//gcc -lstdc++ -std=c++11  <file>

#include "file_reader_two.cpp"
#include "algorithms_three.cpp"
#include <chrono>
//#include <thread>
//#include <mutex>


int main()
{
    //initialize

    std::cout<< "-------------------initializing data----------------" << std::endl;
   
    std::string file_path = "../ca-AstroPh.txt";
    //fill out edge_set and node_map
    Graph G = creat_graph(file_path);
    std::cout<< "Graph created\n" <<"There are " << G.edge_set.size() << " edges" << std::endl;

    //starting program
    std::cout<< "Creating G-trie" <<std::endl;
    //move these to main file
    Trie root; root.n = 0; root.root = nullptr;
    Trie node; node.n = 1; node.root = &root;
    Trie edge; edge.n = 2; edge.root = &root;
    //3-case
    Trie two_star; two_star.n = 3; two_star.root = &root;
    Trie triangle; triangle.n = 3; triangle.root = &root;
    //4-case
    Trie clique; clique.n = 4; clique.root = &root; clique.is_Graph = true;
    Trie chord_cycle; chord_cycle.n = 4; chord_cycle.root = &root; chord_cycle.is_Graph = true;
    Trie t_triangle; t_triangle.n = 4; t_triangle.root = &root; t_triangle.is_Graph = true;
    Trie four_cycle; four_cycle.n = 4; four_cycle.root = &root; four_cycle.is_Graph = true;//ts
    Trie three_star; three_star.n = 4; three_star.root = &root; three_star.is_Graph = true;//ts
    Trie four_path; four_path.n = 4;   four_path.root = &root; four_path.is_Graph = true;//ts

    root.children.push_back(&node);
    node.children.push_back(&edge);
    edge.children.push_back(&triangle);
    edge.children.push_back(&two_star);
    two_star.children.push_back(&four_cycle);
    two_star.children.push_back(&three_star);
    two_star.children.push_back(&four_path);
    triangle.children.push_back(&clique);
    triangle.children.push_back(&chord_cycle);
    triangle.children.push_back(&t_triangle);
    //create adjacency matricies here?Kscnksd;nvksnd;v

    std::map<std::string,std::vector<bool> > adj_map;
    adj_map["root"], root.adj = [];
    adj_map["node"], node.adj = [0];
    adj_map["edge"], edge.adj = [1,0];
    //3 
    adj_map["triangle"], triangle.adj = [1,1,0];
    adj_map["two_star"], two_star.adj = [0,1,0]; // or 100
    //4 t
    adj_map["four_cycle"], four_cycle.adj = [1,1,0,0];
    adj_map["three_star"], three_star.adj = [1,0,0,0];//one node has degree 2
    adj_map["four_path"], four_path.adj = [0,1,0,0]; //all nodes have degree 1
    //4 ts
    adj_map["clique"], clique.adj = [1,1,1,0]; //2-3 edge
    adj_map["chord_cycle"], chord_cycle.adj = [1,1,1,0]; // no 2-3 edge
    adj_map["t_triangle"], t_triangle.adj = [1,0,0,0];

    std::cout<<"Trie initiation komplete"<<std::endl;


    std::cout<< "-------------------Starting  Program----------------" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    GtrieMatch(&root, G);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = duration.count() / 60;
    auto seconds = duration.count() % 60;

    return 0;
} 
