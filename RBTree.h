#ifndef _RBTree_H_
#define _RBTree_H_
#include <iostream>
#define RED 1

using namespace std;


struct Node {
	int data;
	Node* lchild, * rchild, * parent;
	int color;
	Node(int data) {
		this->data = data;
		lchild = NULL;
		rchild = NULL;
		parent = NULL;
		this->color = RED;
	}
};

class C_RBTree {
public:
	C_RBTree();
	~C_RBTree();
	void P_insertValue(int);
	bool P_findNode(int);
	void P_printTree();
	void P_sortTree();
	void P_deleteNode(int);
	Node* P_deleteTree();

private:
	Node* p_root;
	Node* p_recentInsert;

	Node* p_insertValue(Node*, Node*, int);
	void p_colorCorrection(Node*);
	void p_deleteTree(Node*&);
	void p_deleteNode(int, Node*);
	bool p_isBlack(Node*);
	bool p_isRed(Node*);
	Node* p_createNILNode(Node*);
	void p_rotateLeft(Node*);
	void p_rotateRight(Node*);
	void p_printTree(Node*, int);
	Node* p_RRInsertion(Node*, Node*, Node*);
	Node* p_RBInsertion(Node* ,Node* ,Node*, Node*);
	bool p_exists(int);
	Node* p_findNode(int);
	Node* p_findSuccessor(Node*);
	void p_RRDeletion(Node*, Node*);
	Node* p_BRDeletion(Node*&, Node*&, Node*&);
	void p_BBDeletion(Node*, Node*, Node*);
	void p_deletionFixer(Node*);
	void p_convertToNIL(Node*);
};

#endif
