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
		add_pois(); //添加兴趣点
		add_users(); //添加用户
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
		// 析构语义位置、LBS用户空间
		// EC_graph中只保留指针,不需要析构
		for (int i = 0; i < pois.size(); i++) {
			delete(pois[i]);
		}
		for (int i = 0; i < users.size(); i++) {
			delete(users[i]);
		}
    }

	/*
	 *  添加语义信息及用户信息
	 */
	void add_pois()
	{
		// 添加语义位置
		int add_poi_cnt = 1000;
		Poi *pp = nullptr;
		for (int i = 0; i < add_poi_cnt; i++) {
			pp = new Poi(i + 1, 0.5, Semantic_type::hospital);
			edges[i / 2]->add_poi(pp);
			pois.push_back(pp);
		}
	}
	void add_users()
	{
		// 添加用户
		vector<double> u_type1 = { 0.1,0.1,0.5,0.8 }; // 用户模板
		vector<double> u_type2 = { 0.3,0.3,0.5,0.6 };

		int add_user_cnt = 1000;
		LBS_User *pu = nullptr;
		for (int i = 0; i < add_user_cnt; i++) {
			pu = new LBS_User(i + 1, 1, 1, 0.2, u_type1);
			edges[i]->add_user(pu);
			users.push_back(pu);
		}
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
	vector<Poi*> pois; //所有语义点
	vector<LBS_User*> users; //所有用户
};
#endif //C_PLUS_PLUS_PRACTICE_GRAPH_H
