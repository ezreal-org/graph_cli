#ifndef LPPA_SA_H
#define LPPA_SA_H

/*
	�������̱�¶����Asuit,�����㷨�ڿͻ���ִ��
	���û�������������Ĺ�����,�������ѡ�������λ��
	��ѡ���·����㣬���ѡ�����ж���͵���������
	
	Ϊ����Աȣ�s_type_poi>s_require �Ķ���Ϊ��������
*/

#include "../graph.h"
#include <ctime>
#include <cstdlib>
#include <vector>
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
		//��ӡ��ǰ��ͼ״̬
		map_status();
		sa();
	}
	void sa()
	{
		vector<Edge*> edges = p_graph->getEdges();
		LBS_User *pu = nullptr;
		long long u_index = 0;
		for (int i = 0; i < edges.size(); i++) {
			for (int j = 0; j < edges[i]->get_users().size(); j++) {
				pu = edges[i]->get_users()[j];
				users.push_back(edges[i]->get_users()[j]);
				cout << "user :" << pu->get_uid() << " "<< pu->get_k() << " " << pu->get_l() << " " << pu->get_s()  << endl;;
				if (pu->get_uid() > u_index) u_index = pu->get_uid();
			}
			
		}
		cout << u_index << endl;
	}
	void map_status()
	{
		vector<Edge*> edges = p_graph->getEdges();
		int max_user = 0, max_poi = 0;
		int cnt[20];
		for (int i = 0; i<20; i++) cnt[i] = 0;
		for (int i = 0; i <edges.size(); i++) {
			//cout << "edge " << edges[i]->getId() << " user num:" << edges[i]->get_users().size() << " poi num:" << edges[i]->get_pois().size()<< endl;
			if (edges[i]->get_pois().size()>max_poi) max_poi = edges[i]->get_pois().size();
			if (edges[i]->get_users().size() > max_user) max_user = edges[i]->get_users().size();
			cnt[edges[i]->get_users().size()] ++;
		}
		cout << max_poi << "<-poi user-> " << max_user << endl;
		for (int i = 19; i > 0; i--)
		{
			cout << "�û���Ϊ" << i << "�ı��У�" << cnt[i] << endl;
		}
	}
private:
	double anonymization_time_total; //ͳ����Ϣ
	int cnt_of_success;
	int cnt_of_failure;
	vector<double> anonymization_time;
	vector<bool> is_success;
	vector<LBS_User*> users;
	vector<int> open_vetex_cnt;
	Graph *p_graph;
};

#endif