#ifndef LPPA_SA_H
#define LPPA_SA_H

/*
	SA算法隐私模型为 k,θ安全, 采用泰森多边形进行兴趣点划分
	整个流程暴露的是Asuit,匿名算法在客户端执行
	在用户扩大匿名区域的过程中,首先随机选择非敏感位置
	再选择道路交叉点，最后选择流行度最低的敏感语义

	算法本质为，每次加入一个临近兴趣点，直到满足隐私模型
	不能保证l多样性
	为了适应新的隐私目标、方便比较查询效率：
	采用第三方进行匿名，不影响算法本质
	每次匿名扩展时，加入一条临近边而不是一个位置
	s_type_poi_pop/poi_pop_all >s_require 的定义为敏感语义
*/

#include "../graph.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Lppa_sa
{
public:
	Lppa_sa(Graph *&p_graph)
	{
		this->p_graph = p_graph;
		anonymization_time_total = 0;
		cnt_of_failure = 0;
		cnt_of_success = 0;
		l_max = 10;
		//打印当前地图状态
	}
	//保护算法
	void lpp()
	{
		vector<Edge*> edges = p_graph->getEdges();
		LBS_User *pu = nullptr;
		clock_t start, finish;
		start = clock();
		for (int i = 0; i < edges.size(); i++) {
			for (int j = 0; j < edges[i]->get_users().size(); j++) {
				pu = edges[i]->get_users()[j];
				users.push_back(edges[i]->get_users()[j]);
				sa(pu, edges[i]);
			}

		}
		finish = clock();
		double time_cost = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "total time consume:" << time_cost << endl;
		cout << "success:" << cnt_of_success << endl;
		cout << "failure:" << cnt_of_failure << endl;
		//统计
	}
	//为每个用户
	void sa(LBS_User *&pu, Edge *&pe)
	{
		/*cout << "user:" << pu->get_k() << " " << pu->get_l() << " " << pu->get_s() << endl;
		cout << "profile:";
		for (int kk = 0; kk < pu->get_sensitive_vals().size(); kk++) {
			cout << pu->get_sensitive_vals()[kk] << " ";
		}
		cout << endl;*/
		bool is_satisfied = false;
		int k_now = 0, l_now = 0;
		double accumulate_svalue = 0, accumulate_pop = 0;
		vector<Edge*> cloak_set;
		map<Edge*, pair<double,double>> candidate_map;
		set<Node*> inner_node_set;
		vector<double> sensitive_vals = pu->get_sensitive_vals();
		is_satisfied = add_edge_to_cloakset(pu, pe, k_now, l_now, accumulate_svalue, accumulate_pop, cloak_set, candidate_map, inner_node_set);
		for (int i = 1; i < l_max && !is_satisfied; i++) {
			map<Edge*, pair<double,double>>::iterator it_candidate, it_minimal_svalue;
			double minimal_svalue = 0x7fffffff;
			if (candidate_map.size() < 1) break;
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end(); it_candidate++) { //已经计算了的就不用计算
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				if (candidate_svalue/candidate_pop <= pu->get_s()) { //选择非敏感poi
					break;
				}
				if (candidate_svalue < minimal_svalue) {
					minimal_svalue = it_candidate->second.first;
					it_minimal_svalue = it_candidate;
				}
			}
			if (it_candidate != candidate_map.end()) { //找到非敏感poi
				is_satisfied = add_edge_to_cloakset(pu, it_candidate->first, k_now, l_now, accumulate_svalue, accumulate_pop, cloak_set, candidate_map, inner_node_set);
				candidate_map.erase(it_candidate);
			}
			else {
				is_satisfied = add_edge_to_cloakset(pu, it_minimal_svalue->first, k_now, l_now, accumulate_svalue, accumulate_pop, cloak_set, candidate_map, inner_node_set);
				candidate_map.erase(it_minimal_svalue);
			}
		}//end for
		if (is_satisfied) { //成功
			is_success.push_back(true);
			cnt_of_success++;
		}
		else {
			is_success.push_back(false);
			cnt_of_failure++;
		}
		/*cout << (is_satisfied == true ? "success" : "not success") << endl;
		cout << "cloak set info: " << endl;
		for (int i = 0; i < cloak_set.size(); i++) {
			cout << "k:" << cloak_set[i]->get_users().size() << endl;
			for (int j = 0; j < cloak_set[i]->get_pois().size(); j++) {
				cout << "type:" << cloak_set[i]->get_pois()[j]->get_type() << " pop:" << cloak_set[i]->get_pois()[j]->get_pop() << endl;
			}
			cout << "--" << endl;
		}
		if (cloak_set.size() > 5) cout << "####" << endl;
		vv_cloak_sets.push_back(cloak_set);
		cout << "====cloak for this user end.=====" << endl;*/
	}


	bool add_edge_to_cloakset(LBS_User *&pu, Edge *new_edge, int &k, int &l, double &accumulate_svalue, double &accumulate_pop, vector<Edge*> &cloak_set, map<Edge*, pair<double,double>> &candidate_set, set<Node*> &inner_node_set)
	{
		//更新匿名集和候选集
		vector<double> sensitive_vals = pu->get_sensitive_vals();
		cloak_set.push_back(new_edge);
		Node *pn1, *pn2;
		vector<Edge*> adj_edges;
		pn1 = new_edge->getNode1(); //加入一条边,增加一个新顶点或者不变
		pn2 = new_edge->getNode2(); //只影响刚加入的部分
		if (inner_node_set.find(pn1) == inner_node_set.end()) {
			inner_node_set.insert(pn1);
			adj_edges = pn1->getAdjEdges();
			for (int i = 0; i < adj_edges.size(); i++) { //计算邻居边对该用户的敏感情况
				Edge *candidate_edge = adj_edges[i];
				if (candidate_edge != new_edge) {
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					vector<Poi*> e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					if (candidate_edge_pop < 0.0001) { candidate_edge_svalue = pu->get_s() + 0.001; candidate_edge_pop = 1; } //空白语义第二优先
					candidate_set[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop); // 用于判断敏感率 < θ
				}
			}
		}
		if (inner_node_set.find(pn2) == inner_node_set.end()) {
			inner_node_set.insert(pn2);
			adj_edges = pn2->getAdjEdges();
			for (int i = 0; i < adj_edges.size(); i++) {
				Edge *candidate_edge = adj_edges[i];
				if (candidate_edge != new_edge) {
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					vector<Poi*> e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_set[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop);
				}
			}
		}
		//计算当前状态
		k += new_edge->get_users().size();
		l++;
		vector<Poi*> e_pois = new_edge->get_pois();
		for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
			int poi_type = (int)e_pois[j]->get_type();
			accumulate_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
			accumulate_pop += e_pois[j]->get_pop();
		}
		if (k < pu->get_k() || l < pu->get_l() || accumulate_svalue / accumulate_pop > pu->get_s()) return false;
		return true;
	}
private:
	vector<bool> is_success;
	vector<LBS_User*> users;
	vector<vector<Edge*>> vv_cloak_sets;
	//统计信息
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //将减去环数
	int l_max; //匿名集最多选择l_max条边,否则失败
	Graph *p_graph;
};

#endif