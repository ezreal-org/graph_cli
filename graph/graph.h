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

using namespace std;
class Graph{
public:
    Graph(ifstream &f_nodes,ifstream &f_edges)
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
				     
            pNode = new Node(node_id,x,y);
            nodes.push_back(pNode);
            nodes_map[node_id] = pNode;
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
            pEdge = new Edge(e_id,pNode1,pNode2,edge_class,edge_len);
            pNode1->addEdge(pEdge);
            pNode2->addEdge(pEdge);
            edges.push_back(pEdge);
            //++numOfEdges;
            //edges_map[e_id] = pEdge;
        }
        f_edges.close();
		add_pois(); //�����Ȥ��
		//add_users(); //����û�
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

	/*
	 *  ���������Ϣ���û���Ϣ
	 */
	void add_pois()
	{
		//��ͼ�������Ϣ��map_info
		//��ͼedge class��0~6�߸��ȼ�
		int arr_edge_keys[] = { 0,1,2,3,4,5,6 };
		int arr_edge_weight[] = { 10,8,6,4,3,2,1 }; //��ʼ��Ȩֵ,���ݱߵ���Ŀ�ٵ���,������Ȩֵ=��ʼֵ*����
		vector<int> edge_keys(arr_edge_keys, arr_edge_keys + 7);
		vector<int> edge_weights(arr_edge_weight, arr_edge_weight + 7);

		vector<vector<Edge*>> vv_edges(7);  //��¼ÿ�����ĸ���
		for (int i = 0; i<edges.size(); i++) {
			int edge_class = edges[i]->getEdgeClass();
			vv_edges[edge_class].push_back(edges[i]);
		}
		for (int i = 0; i < vv_edges.size(); i++) {
			edge_weights[i] *= vv_edges[i].size();
		}

		WRandom_Generator wr_generator(edge_keys, edge_weights);//�������·��Ȩֵ�������������
		//���ҽԺ
		add_poi_operate(1000, Semantic_type::hospital, wr_generator, vv_edges);
		//���ѧУ
		add_poi_operate(1000, Semantic_type::school, wr_generator, vv_edges);
	}
	void add_poi_operate(int cnt,Semantic_type s_type, WRandom_Generator &generator, const vector<vector<Edge*>> &vv_edges)
	{
		int add_poi_cnt = cnt;
		Poi *pp = nullptr;
		for (int i = 1; i <= add_poi_cnt; i++) {
			double pop = (double)generator.get_next_r() / generator.get_random_max();
			pp = new Poi(i, pop, s_type);
			//�������ѡһ���ȼ�,������̽�����·��Ȩֵ
			int edge_class = generator.get_next_wr();
			// �ٴ�ĳ���������һ��
			int edge_class_cnt = vv_edges[edge_class].size();
			// ʹ�ô������������
			int random_edge_index = generator.get_next_r() % edge_class_cnt;
			vv_edges[edge_class][random_edge_index]->add_poi(pp);
			//cout << "edge class" << vv_edges[edge_class][random_edge_index]->getEdgeClass() << endl;
			pois.push_back(pp);
		}
	}
	void add_users()
	{
		// ����û�
		vector<double> u_type1 = { 0.1,0.1,0.5,0.8 }; // �û�ģ��
		vector<double> u_type2 = { 0.3,0.3,0.5,0.6 };

		int add_user_cnt = 1000;
		LBS_User *pu = nullptr;
		for (int i = 0; i < add_user_cnt; i++) {
			pu = new LBS_User(i + 1, 1, 1, 0.2, u_type1);
			edges[i]->add_user(pu);
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
