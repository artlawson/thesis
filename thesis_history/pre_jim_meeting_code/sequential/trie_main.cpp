//trie_main
//gcc -lstdc++ -std=c++17  <file>
#include "file_reader_two.cpp"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
//#include <thread>
//#include <mutex>


int main()
{
    //initialize
    std::cout<< "-------------------initializing data----------------" << std::endl;
   
    //std::string file_path = "../txt/ca-AstroPh.txt";
    std::string file_path = "../txt/p2p-Gnutella09.txt";
    //fill out edge_set and node_map
    Graph G;
    creat_graph(file_path, G);//defined in file_reader_two
    
    std::cout<< "Graph created\n" <<"There are " << G.edge_set.size() << " edges and " << G.verticies.size() << " verticies!"<< std::endl;

    //starting program
    std::cout<< "Creating G-trie" <<std::endl;
    std::shared_ptr<Trie> root = create_trie();//defined in file_reader_two
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::cout<<"Trie initiation komplete!"<<std::endl;

    std::cout<< "\n-------------------Starting  Program!----------------" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    GtrieMatch(root, G,GDD);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = (duration.count() / 60) % 60;
    auto seconds = duration.count() % 60;
    auto hours = duration.count() / 360;

    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor "<<hours << "hours, " << minutes<< " minutes and " << seconds << " seconds" <<std::endl;

    std::cout<<"\n ------------------Here are the results--------------"<<std::endl;
    
    for (auto kv : GDD){
        std::string key = kv.first;
        int value = kv.second.size();
        
        std::cout<<value <<" " <<key << "s" <<std::endl;
    }
    //send an email to confirm program termination
    int status;
    status = system(R"(echo "this is the body" | mail -s "this is the subject" "artlawson318@gmail.com")");

    return 318;
} 
