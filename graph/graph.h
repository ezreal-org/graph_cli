//
// Created by jayce on 09/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_GRAPH_H
#define C_PLUS_PLUS_PRACTICE_GRAPH_H
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <iostream>
#include "graph_node.h"
#include "graph_edge.h"
#include "random_generator.h"
#include "util.h"

using namespace std;
class Graph{
public:
    Graph(ifstream &f_nodes,ifstream &f_edges,string config_path)
    {
        Node *pNode;
        Edge *pEdge;
		x_min = y_min = 0x7fffffff;
		x_max = y_max = 0;
        // init nodes
        cout << "init nodes" << endl;
        long long node_id;
        double x,y;
        char end_char[20];
        if(f_nodes.bad()){
            cout << "file open failure" << endl;
            return;
        }
		int node_index = 0;
        while(!f_nodes.eof()){
            //oldenburgGen map data have a end_char each line.
            f_nodes >> node_id >> x >> y >> end_char;
			if (x < x_min) {
				x_min = x;
			}
			if (x > x_max) {
				x_max = x;
			}
			if (y < y_min) {
				y_min = y;
			}
			if (y > y_max) {
				y_max = y;
			}
            pNode = new Node(node_index,node_id,x,y);
            nodes.push_back(pNode);
            nodes_map[node_id] = pNode;
			node_index++;
            //++numOfNodes;
			//cout << "node:"<<pNode->getId() << endl;
        }
        f_nodes.close();

        // init edges
        cout << "init edges" << endl;
        long long e_id;
        long long node1_id,node2_id;
        int edge_class;
        double edge_len;
        Node *pNode1,*pNode2;
        if(f_edges.bad()){
            cout << "file open failure" << endl;
            return;
        }
		int edge_id = 0;
        while(!f_edges.eof()){
            f_edges >> e_id >> node1_id  >> node2_id >> edge_class >>  end_char;
            pNode1 = getNode(node1_id);
            pNode2 = getNode(node2_id);
            double x1,y1,x2,y2;
            x1 = pNode1->getX();
            y1 = pNode1->getY();
            x2 = pNode2->getX();
            y2 = pNode2->getY();
            edge_len = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
            pEdge = new Edge(edge_id++,pNode1,pNode2,edge_class,edge_len);
            pNode1->addEdge(pEdge);
            pNode2->addEdge(pEdge);
            edges.push_back(pEdge);
            //++numOfEdges;
            //edges_map[e_id] = pEdge;
        }
        f_edges.close();

		vector<vector<Edge*>> vv_edges(7); //��¼ÿ�����ĸ���
		WRandom_Generator *p_generator = init_weighted_random_generator(vv_edges);
		add_pois(p_generator,vv_edges,config_path); //�����Ȥ��
		add_users(p_generator,vv_edges,config_path); //����û�
		delete(p_generator);
        cout << "graph init done" << endl;
    }

    ~Graph()
    {
        for(int i=0;i<nodes.size();i++){
            delete(nodes[i]);
        }
        for(int i=0;i<edges.size();i++){
            delete(edges[i]);
        }
		// ��������λ�á�LBS�û��ռ�
		// EC_graph��ֻ����ָ��,����Ҫ����
		for (int i = 0; i < pois.size(); i++) {
			delete(pois[i]);
		}
		for (int i = 0; i < users.size(); i++) {
			delete(users[i]);
		}
    }

	WRandom_Generator* init_weighted_random_generator(vector<vector<Edge*>> &vv_edges)
	{
		//��ͼ�������Ϣ��map_info
		//��ͼedge class��0~6�߸��ȼ�
		int arr_edge_keys[] = { 0,1,2,3,4,5,6 };
		int arr_edge_weight[] = { 10,8,6,4,3,2,1 }; //��ʼ��Ȩֵ,���ݱߵ���Ŀ�ٵ���,������Ȩֵ=��ʼֵ*����
		vector<int> edge_keys(arr_edge_keys, arr_edge_keys + 7);
		vector<int> edge_weights(arr_edge_weight, arr_edge_weight + 7);

		for (int i = 0; i<edges.size(); i++) {
			int edge_class = edges[i]->getEdgeClass();
			vv_edges[edge_class].push_back(edges[i]);
		}
		for (int i = 0; i < vv_edges.size(); i++) {
			edge_weights[i] *= vv_edges[i].size();
		}
		//�������·��Ȩֵ�������������
		WRandom_Generator*p_generator =  new WRandom_Generator(edge_keys, edge_weights);
		return p_generator;
	}
	/*
	 *  ���������Ϣ���û���Ϣ�����ʹ�û���·��Ȩ�ص�������
	 */
	void add_pois(WRandom_Generator *&generator, const vector<vector<Edge*>> &vv_edges,string config_path)
	{
		Graph_Config gc(config_path);
		map<string, int> pois_to_add = gc.parse_pois_section();
		map<string, int>::iterator it_pois;
		for (it_pois = pois_to_add.begin(); it_pois != pois_to_add.end(); it_pois++) {
			Semantic_type st;
			if (it_pois->first == "school") {
				st = Semantic_type::school;
			}
			else if (it_pois->first == "hospital") {
				st = Semantic_type::hospital;
			}
			else if (it_pois->first == "bar") {
				st = Semantic_type::bar;
			}
			else if (it_pois->first == "company") {
				st = Semantic_type::company;
			}
			else if (it_pois->first == "market") {
				st = Semantic_type::market;
			}
			else if (it_pois->first == "residence") {
				st = Semantic_type::residence;
			}
			add_poi_operate(it_pois->second, st, generator, vv_edges);
		}
	}
    //����û�
	void add_users(WRandom_Generator *&generator, const vector<vector<Edge*>> &vv_edges,string config_path)
	{
		Graph_Config gc(config_path);
		map<string, pair<vector<double>, int>> users_to_add = gc.parse_user_section();
		map<string, pair<vector<double>, int>>::iterator it_add_users;
		for (it_add_users = users_to_add.begin(); it_add_users != users_to_add.end(); it_add_users++) {
			add_user_operate(it_add_users->second.second, it_add_users->second.first, generator, vv_edges);
		}
	}
	void add_poi_operate(int cnt, Semantic_type s_type, WRandom_Generator *&generator, const vector<vector<Edge*>> &vv_edges)
	{
		int add_poi_cnt = cnt;
		Poi *pp = nullptr;
		for (int i = 0; i < add_poi_cnt; i++) {
			double pop = (double)generator->get_next_r() / generator->get_random_max();
			long long poi_id = pois.size()+1;
			pp = new Poi(poi_id, pop, s_type);
			//�������ѡһ���ȼ�,������̽�����·��Ȩֵ
			int edge_class = generator->get_next_wr();
			// �ٴ�ĳ���������һ��
			int edge_class_cnt = vv_edges[edge_class].size();
			// ʹ�ô������������
			int random_edge_index = generator->get_next_r() % edge_class_cnt;
			vv_edges[edge_class][random_edge_index]->add_poi(pp);
			//cout << "edge class" << vv_edges[edge_class][random_edge_index]->getEdgeClass() << endl;
			pois.push_back(pp);
		}
	}
	void add_user_operate(int cnt, const vector<double> &u_profile, WRandom_Generator *&generator, const vector<vector<Edge*>> &vv_edges)
	{
		int add_user_cnt = cnt;
		LBS_User *pu = nullptr;
		for (int i = 0; i < add_user_cnt; i++) {
			//double s_require = 0.2 + ((double)generator->get_next_r() / generator->get_random_max()) * (0.8-0.2); // 0.2~0.8
			double s_require = 0.2;
			long long u_id = users.size() + 1;
			pu = new LBS_User(u_id, 20, 5, s_require, u_profile);
			//�������ѡһ���ȼ�,������̽�����·��Ȩֵ
			int edge_class = generator->get_next_wr();
			// �ٴ�ĳ���������һ��
			int edge_class_cnt = vv_edges[edge_class].size();
			// ʹ�ô������������
			int random_edge_index = generator->get_next_r() % edge_class_cnt;
			vv_edges[edge_class][random_edge_index]->add_user(pu);
			//cout << "edge class" << vv_edges[edge_class][random_edge_index]->getEdgeClass() << endl;
			users.push_back(pu);
		}
	}
	const vector<Poi*>& get_pois()
	{
		return pois;
	}
	const vector<LBS_User*>& get_users()
	{
		return users;
	}
    const vector<Edge*>& getEdges()
    {
        return edges;
    }
    const vector<Node*>& getNodes()
    {
        return nodes;
    }
//    int getNumofEdges()
//    {
//        return numOfEdges;
//    }
//    int getNumofNodes()
//    {
//        return numOfNodes;
//    }
    Node* getNode(long long node_id)
    {
        if(nodes_map.find(node_id)!=nodes_map.end()){
            return nodes_map[node_id];
        }
    }
	double get_xmin()
	{
		return x_min;
	}
	double get_xmax()
	{
		return x_max;
	}
	double get_ymin()
	{
		return y_min;
	}
	double get_ymax()
	{
		return y_max;
	}
//    Edge* getEdge(long edge_id)
//    {
//        if(edges_map.find(edge_id)!=edges_map.end()){
//            return edges_map.at(edge_id);
//        }
//    }
private:
    vector<Edge*> edges;
    vector<Node*> nodes;
    map<long long,Node*> nodes_map;
    //map<long,Edge*> edges_map;
    //int numOfEdges;
    //int numOfNodes;
	double x_min, x_max, y_min, y_max;
	vector<Poi*> pois; //���������
	vector<LBS_User*> users; //�����û�
};
#endif //C_PLUS_PLUS_PRACTICE_GRAPH_H
