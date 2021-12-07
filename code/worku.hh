#ifndef __WORKU_H_
#define __WORKU_H_

#include <vector>
#include <memory>
#include "classes_two.hh"

bool check_connections(int vp, std::vector<int> &v_used, Graph &G, std::shared_ptr<Trie> T);
void parallel_candidates_creator(int p_id,std::vector<int> &cand, std::vector<int> &v_used, Graph G);
std::vector<int> parallel_Matching_verticies(int p_id, std::shared_ptr<Trie> T,Graph &G, std::vector<int> &v_used);
bool check_message_threshold(Graph &G);
void check_messages(WorkUnit* W, Graph &G);
void ask_for_more_work(WorkUnit* W, Graph &G);
void parallel_match(int p_id, Graph &G, WorkUnit &W);
void resume_work(WorkUnit &W, Graph &G);
void recursive_process(WorkUnit* W, Graph &G);
void parallel_count(int p_id,WorkUnit &W, std::shared_ptr<Trie> T, Graph &G);

#endif
