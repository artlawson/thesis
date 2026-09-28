//p_main
//gcc -lstdc++ -std=c++17  <file>
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "worku.cpp"
//#include <mutex>


int main()
{
    //initialize
    std::cout<< "-------------------initializing data----------------" << std::endl;
   
    std::string file_path = "../txt/p2p-Gnutella09.txt";
    Graph G;
    create_graph(file_path,G);//defined in file_reader_two
    std::cout<< "Graph created\n" <<"There are " << G.edge_set.size() << " edges and " << G.verticies.size() << " verticies!"<< std::endl;

    //starting program
    std::cout<< "Creating G-trie" <<std::endl;
    std::shared_ptr<Trie> root = create_trie();//defined in file_reader_two
    std::cout<<"Trie initiation komplete!"<<std::endl;
    /*later make globals
    GDD
    G

    */
   
    G.P = 1;

    std::vector<std::thread> threads;

    std::cout<< "\n-------------------Starting  Program!----------------" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < G.P; i++){
        //this is where meat of Parallel Count would happen
        auto lm = [i ,root,&G]{ parallel_count(i,root, G);}; //t_triangle
        

        threads.push_back(std::thread{lm}); 
    }

    
    // Wait for each to complete
    std::cout<<"running on " << threads.size()<< " threads" <<std::endl;
    for (std::thread& t : threads) {
        //could print time of each thread finishing
        t.join();
    }
    //parallel_count(0,root,G);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = duration.count() / 60;
    auto seconds = duration.count() % 60;
    auto hours = minutes / 60;

    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor "<<hours << "hours, " << minutes<< " minutes and " << seconds << " seconds" <<std::endl;

    std::cout<<"\n ------------------Here are the results--------------"<<std::endl;
    
    G.GDD.print();
    //send an email to confirm program termination
    //int status;
    //status = system(R"(echo "your thesis code has terminated execution!" | mail -s "Thesis Execution Complete" "artlawson318@gmail.com")");

    return 318;
} 
