#ifndef __FILE_READER_TWO_
#define __FILE_READER_TWO_

#include <string>
#include <set>
#include <utility>
#include "classes_two.hh"

bool is_number(const std::string &s);
int get_second_number(std::string &s);
std::vector<int> splitter(std::string &s, std::string delim);
void read_edges(std::string file,
                std::set<std::pair<int,int> > &edge_set,
                std::unordered_map<int,Vertex> &vert_map, 
                std::unordered_map<int, std::set<int> > &node_map);
void create_graph(std::string file, Graph &new_graph);
std::shared_ptr<Trie> create_trie();

#endif
