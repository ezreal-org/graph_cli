#ifndef LPPA_UTIL_H
#define LPPA_UTIL_H

#include "../edge_cluster_node.h"
#include <vector>
using namespace std;

template<typename T>
bool vector_find(vector<T> &v_input,T &value)
{
	vector<T>::iterator it_v;
	for (it_v = v_input.begin(); it_v != v_input.end(); it_v++) {
		if (*it_v == value) {
			break;
		}
	}
	if (it_v == v_input.end()) return false;
	return true;
}

bool vv_find(vector<EC_Node*> &v_input, EC_Node* value)
{
	vector<EC_Node *>::iterator it_v;
	for (it_v = v_input.begin(); it_v != v_input.end(); it_v++) {
		if (*it_v == value) {
			break;
		}
	}
	if (it_v == v_input.end()) return false;
	return true;
}
#endif