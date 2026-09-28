//testing for creating g-trie
#include <cassert>
#include "file_reader_two.cpp"


bool triangle(std::shared_ptr<Trie> root){
    //triangle doing as expected, maybe fix adj for other
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::string file_path = "../txt/triangle.txt";
    Graph G = creat_graph(file_path);
    assert(G.edge_set.size() == 3);
    assert(G.verticies.size() == 3);
    GtrieMatch(root,G, GDD);
    assert(GDD["two_star"].size() == 0);
    assert(GDD["triangle"].size() >= 1);
    return true;
}

bool two_star(std::shared_ptr<Trie> root){
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::string file_path = "../txt/two_star.txt";
    Graph G = creat_graph(file_path);
    assert(G.edge_set.size() == 2);
    assert(G.verticies.size() == 3);
    GtrieMatch(root,G, GDD);
    assert(GDD["triangle"].size() == 0);
    assert(GDD["two_star"].size() >= 1);
    return true;
}

bool distinct_triangle(std::shared_ptr<Trie> root){
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::string file_path = "../txt/two_triangle.txt";
    Graph G = creat_graph(file_path);
    assert(G.edge_set.size() == 7);
    assert(G.verticies.size() == 6);
    GtrieMatch(root,G, GDD);
    assert(GDD["two_star"].size() >= 1);
    assert(GDD["triangle"].size() >= 2);
    return true;
}

bool disjoint_triangle(std::shared_ptr<Trie> root){
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::string file_path = "../txt/disjoint_triangle.txt";
    Graph G = creat_graph(file_path);
    assert(G.edge_set.size() == 5);
    assert(G.verticies.size() == 4);
    GtrieMatch(root,G, GDD);
    assert(GDD["two_star"].size() >= 1);
    assert(GDD["triangle"].size() >= 2);
    return true;
}

bool four_path(std::shared_ptr<Trie> root){
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::string file_path = "../txt/four_path.txt";
    Graph G = creat_graph(file_path);
    assert(G.edge_set.size() == 3);
    assert(G.verticies.size() == 4);
    GtrieMatch(root,G, GDD);
    assert(GDD["triangle"].size() == 0);
    assert(GDD["two_star"].size() >= 1);
    assert(GDD["four_path"].size() >= 1);
    return true;
}

bool clique(std::shared_ptr<Trie> root){
    std::unordered_map<std::string, std::set<std::vector<int> > > GDD;
    std::string file_path = "../txt/clique.txt";
    Graph G = creat_graph(file_path);
    assert(G.edge_set.size() == 6);
    assert(G.verticies.size() == 4);
    GtrieMatch(root,G, GDD);
    assert(GDD["triangle"].size() >= 1);
    assert(GDD["two_star"].size() == 0);
    assert(GDD["clique"].size() >= 1);
    return true;
}


int main(){
    std::shared_ptr<Trie> root = create_trie();

    //1st
    if (triangle(root)){
        std::cout<< "Triangle passed!"<<std::endl;
    }

    //2nd
    if (two_star(root)){
        std::cout<< "two_star passed!"<<std::endl;
    }

    //3rd
    if (distinct_triangle(root)){
        std::cout<< "distinct triangle passed!"<<std::endl;
    }
    //4th
    if (disjoint_triangle(root)){
        std::cout<< "disjoint triangle passed!"<<std::endl;
    }
    //5th
    if (four_path(root)){
        std::cout<< "four_path passed!"<<std::endl;
    }

    //6th
    if (clique(root)){
        std::cout<< "clique passed!"<<std::endl;
    }


    

    return 318;
}