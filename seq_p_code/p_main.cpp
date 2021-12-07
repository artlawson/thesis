//p_main
//gcc -lstdc++ -std=c++17  <file>
#include <iostream>
#include <chrono>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include "file_reader_two.hh"
#include "classes_two.hh"
#include "worku.hh"


#include <random>
#include <fstream>
void txt_generator(int n){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,n-1); // distribution in range [x, y]
    // Create and open a text file
    std::ofstream MyFile("../txt/random.txt");
    for (int i = 0; i < (n); i++){
        int x = dist(rng);
        int y = dist(rng);
        while (y == x){y = dist(rng);}
        // Write to the file
        MyFile << std::to_string(x) << "    " << std::to_string(y) << "\n";
    }
    
    // Close the file
    MyFile.close();
    
}


int main()
{
    //initialize
    std::cout<< "-------------------initializing data----------------" << std::endl;
    //txt_generator(1008000); std::cout<< "creating random graph" <<std::endl;
    //std::string file_path = "../txt/random.txt";
    //std::string file_path = "../txt/ca-AstroPh.txt";
    //std::string file_path = "../txt/ca-GrQc.txt";
   //std::string file_path = "../txt/ca-HepPh.txt";
    std::string file_path = "../txt/p2p-Gnutella09.txt";
   //std::string file_path = "../txt/ca-CondMat.txt";
    //std::string file_path = "../txt/ca-HepTh.txt";
    Graph G; create_graph(file_path, G);//defined in file_reader_two
    std::cout<< "Graph created from "<< file_path << "\n There are " << G.edge_set.size() << " edges and " << G.v << " verticies!"<< std::endl;

    //starting program
    std::cout<< "Creating G-trie" <<std::endl;
    std::shared_ptr<Trie> root = create_trie();//defined in file_reader_two
    std::shared_ptr<Trie> start_node = root->children[0]; //fix issue of current node being behind what program is actually checking for
    std::cout<<"Trie initiation komplete!"<<std::endl;
   
    G.P = 1;
    if (G.P > G.verticies.size()){
        std::cout << "Uh, oh, more threads requested than vertices! Setting P = |V|"<<std::endl;
        G.P = G.verticies.size();
    }
     //start with high number, update accordingly
    std::vector<WorkUnit> units(G.P);
    std::vector<std::thread> threads;


    std::cout<< "\n-------------------Starting  Program!----------------" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    //parallel_count(0,start_node, G);
    
    for (int i = 0; i < G.P; i++){
        auto lm = [i,&units,start_node,&G]{ parallel_count(i,units[i],start_node, G);}; //t_triangle
        threads.push_back(std::thread{lm}); 
    }

    // Wait for each to complete
    std::cout<<"running on " << threads.size()<< " threads" <<std::endl;
    for (std::thread& t : threads) {
        t.join();
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    auto minutes = (duration.count() / 60) % 60;
    auto seconds = duration.count() % 60;
    auto hours = (duration.count() / 60) / 60;

    std::cout<<"\n ------------------Time's up!--------------"<<std::endl;

    std::cout<< "This operation took \n" << duration.count() << " seconds"
    << "\nor "<<hours << " hours, " << minutes<< " minutes and " << seconds << " seconds" <<std::endl;


    std::cout<<"\n ------------------Aggregating results---------------"<<std::endl;
    for (WorkUnit w : units){
        G.GDD.combine(w.local_GDD);
    }
    std::cout<<"\n ------------------Here are the results--------------"<<std::endl;
    
    G.GDD.print();
    //send an email to confirm program termination
    //int status;
    //status = system(R"(echo "your thesis code has terminated execution!" | mail -s "Thesis Execution Complete" "artlawson318@gmail.com")");

    return 318;
} 
