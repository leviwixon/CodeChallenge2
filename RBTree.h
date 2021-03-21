#ifndef _RBTree_H_
#define _RBTree_H_
#include <iostream>

using namespace std;

struct Node {
	int data;
	Node* lchild, * rchild, * parent;
	int color;
};

class C_RBTree {
public:
	C_RBTree();
	~C_RBTree();
	void P_insertValue(int);
	bool P_findNode(int);
	void P_printTree();
	void P_sortTree();
	Node* P_deleteTree(Node*);

private:
	Node* p_root;

	Node* p_insertValue(Node*, int);
	void p_updateColors(Node*);
	Node* p_setParent(Node*,int);
	bool p_isBlack(Node*);
	bool p_isRed(Node*);
	void p_rotateLeft(Node*, Node*, Node*);
	void p_rotateRight(Node*, Node*, Node*);
};

#endif
