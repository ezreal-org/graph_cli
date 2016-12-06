#ifndef LPPA_SA_ENHANCED_H
#define LPPA_SA_ENHANCED_H

/*
SA�㷨��˽ģ��Ϊ k,�Ȱ�ȫ, ����̩ɭ����ν�����Ȥ�㻮��
�������̱�¶����Asuit,�����㷨�ڿͻ���ִ��
���û�������������Ĺ�����,�������ѡ�������λ��-����ȱʧֵ
��ѡ���·����㣬���ѡ�����ж���͵���������

�㷨����Ϊ��ÿ�μ���һ���ٽ���Ȥ�㣬ֱ��������˽ģ��
���ܱ�֤l������
Ϊ����Ӧ�µ���˽Ŀ�ꡢ����Ƚϲ�ѯЧ�ʣ�
���õ�����������������Ӱ���㷨����
ÿ��������չʱ������һ���ٽ��߶�����һ��λ��
s_type_poi_pop/poi_pop_all >s_require �Ķ���Ϊ��������
*/

#include "../graph.h"
#include "util.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Lppa_sa_e
{
public:
	Lppa_sa_e(Graph *&p_graph)
	{
		this->p_graph = p_graph;
		anonymization_time_total = 0;
		cnt_of_failure = 0;
		cnt_of_success = 0;
		l_max = 100;
	}
	//�����㷨
	void lpp()
	{
		const vector<Edge*> &edges = p_graph->getEdges();
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
		//ͳ��
		//��һ�齫����������ʱ�䣬ֻ�����ڷ����㷨����
		//���Žڵ㴦������ʱδ��״̬,ֻ��Ϊ����ָ��
		cout << "calculate open vetex begin:" << endl;
		set<Node*> node_set;
		set<Node*>::iterator it_node_set;
		vector<Edge*> adj_edges, cloak_set;
		vector<Edge*>::iterator it_cloak_set;
		Node *pn1, *pn2;
		int all_openvetex_cnt = 0, all_edge_size = 0;

		int user_cnt = vv_cloak_sets.size();
		for (int i = 0; i < vv_cloak_sets.size(); i++) {
			node_set = vs_inner_nodes[i]; //��Ӧ��ض��㼯
			cloak_set = vv_cloak_sets[i]; //������
			all_edge_size += cloak_set.size();
			int cnt_of_circle = 0, cnt_of_openvetex = 0;
			for (it_node_set = node_set.begin(); it_node_set != node_set.end(); it_node_set++) {
				adj_edges = (*it_node_set)->getAdjEdges();
				for (int j = 0; j < adj_edges.size(); j++) { //�ж������ٽ����Ƿ���������
					if (!vector_find(cloak_set, adj_edges[j])) { //û�ҵ�,�������ж����ڣ�
						cnt_of_openvetex++;
						break;
					}
				}
			}
			open_vetex_cnt.push_back(cnt_of_openvetex);
			all_openvetex_cnt += cnt_of_openvetex;
			//cout << "open vetex: "<< cnt_of_openvetex << endl;
			if (cnt_of_openvetex > 60) cout << "###" << endl;
		}
		cout << "average openvetex: " << (double)all_openvetex_cnt / user_cnt << endl;
		cout << "average cloak size: " << (double)all_edge_size / user_cnt << endl;

	}
	//Ϊÿ���û�
	void sa(LBS_User *&pu,  Edge *pe)
	{
		bool is_satisfied = false;
		k = 0, l = 0;
		accumulate_svalue = 0, accumulate_pop = 0;
		vector<Edge*> cloak_set;
		map<Edge*, pair<double, double>> candidate_map;
		set<Node*> inner_node_set;
		const vector<double> &sensitive_vals = pu->get_sensitive_vals();
		is_satisfied = add_edge_to_cloakset(pu, pe, cloak_set, candidate_map, inner_node_set);
		for (int i = 1; i < l_max && !is_satisfied; i++) {
			map<Edge*, pair<double, double>>::iterator it_candidate, it_minimal_missed_pure_semantic;
			double minimal_missed_pure_senmantic = 0x7fffffff;
			if (candidate_map.size() < 1) break;
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end(); it_candidate++) { //�Ѿ������˵ľͲ��ü���
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				double missed_value = (candidate_svalue + accumulate_svalue) / pu->get_s() - (accumulate_pop + candidate_pop);
				//ѡ������ȱʧֵ��С��
				if (missed_value < minimal_missed_pure_senmantic) {
					minimal_missed_pure_senmantic = missed_value;
					it_minimal_missed_pure_semantic = it_candidate;
				}
			}

			is_satisfied = add_edge_to_cloakset(pu, it_minimal_missed_pure_semantic->first, cloak_set, candidate_map, inner_node_set);
			candidate_map.erase(it_minimal_missed_pure_semantic);

		}//end for
		if (is_satisfied) { //�ɹ�
			is_success.push_back(true);
			cnt_of_success++;
		}
		else {
			is_success.push_back(false);
			cnt_of_failure++;
		}
		vv_cloak_sets.push_back(cloak_set);
		vs_inner_nodes.push_back(inner_node_set);
	}


	bool add_edge_to_cloakset(LBS_User *&pu, Edge *new_edge,vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map, set<Node*> &inner_node_set)
	{
		//�����������ͺ�ѡ��
		const vector<double> &sensitive_vals = pu->get_sensitive_vals();
		cloak_set.push_back(new_edge);
		Node *pn1, *pn2;
		pn1 = new_edge->getNode1(); //����һ����,����һ���¶�����߲���
		pn2 = new_edge->getNode2(); //ֻӰ��ռ���Ĳ���
		if (inner_node_set.find(pn1) == inner_node_set.end()) {
			inner_node_set.insert(pn1);
			vector<Edge*> &adj_edges = pn1->getAdjEdges();
			for (int i = 0; i < adj_edges.size(); i++) { //�����ھӱ߶Ը��û����������
				Edge *candidate_edge = adj_edges[i];
				if (candidate_edge != new_edge) {
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					const vector<Poi*> &e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //�����Ȥ��
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_map[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop); // �����ٴ��������Ȥ������
				}
			}
		}
		if (inner_node_set.find(pn2) == inner_node_set.end()) {
			inner_node_set.insert(pn2);
			vector<Edge*> &adj_edges = pn2->getAdjEdges();
			for (int i = 0; i < adj_edges.size(); i++) {
				Edge *candidate_edge = adj_edges[i];
				if (candidate_edge != new_edge) {
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					const vector<Poi*> &e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //�����Ȥ��
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_map[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop);
				}
			}
		}
		//���㵱ǰ״̬
		k += new_edge->get_users().size();
		l++;
		double candidate_svalue = 0.0, candidate_pop = 0.0;
		if (candidate_map.find(new_edge) == candidate_map.end()) { //��һ��ִ��
			const vector<Poi*> &e_pois = new_edge->get_pois();
			for (int j = 0; j < e_pois.size(); j++) { //�����Ȥ��
				int poi_type = (int)e_pois[j]->get_type();
				candidate_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
				candidate_pop += e_pois[j]->get_pop();
			}
		}
		else {
			candidate_svalue = candidate_map.find(new_edge)->second.first;
			candidate_pop = candidate_map.find(new_edge)->second.second;
		}
		accumulate_svalue += candidate_svalue;
		accumulate_pop += candidate_pop;
		if (k < pu->get_k() || l < pu->get_l() || (accumulate_pop > 0.001 && accumulate_svalue / accumulate_pop > pu->get_s())) return false;
		//cout << "find satisfied" << endl;
		return true;
	}
private:
	int k, l;
	double accumulate_svalue = 0, accumulate_pop = 0;
	vector<bool> is_success;
	vector<LBS_User*> users;
	vector<vector<Edge*>> vv_cloak_sets;
	vector<set<Node*>> vs_inner_nodes;
	//ͳ����Ϣ
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //��������Ȩֵ��ȥ����
	int l_max; //���������ѡ��l_max����,����ʧ��
	Graph *p_graph;
};

#endif