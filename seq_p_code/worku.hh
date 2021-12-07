#ifndef __WORKU_H_
#define __WORKU_H_

#include <vector>
#include <memory>
#include "classes_two.hh"

bool check_connections(int vp, std::vector<int> &v_used, Graph &G, std::shared_ptr<Trie> T);
void parallel_candidates_creator(int p_id,std::vector<int> &cand, std::vector<int> &v_used, Graph G);
std::vector<int> parallel_Matching_verticies(int p_id, std::shared_ptr<Trie> T,Graph &G, std::vector<int> &v_used);
void parallel_match(int p_id, Graph &G, WorkUnit &W);
void parallel_count(int p_id,WorkUnit &W, std::shared_ptr<Trie> T, Graph &G);

#endif
