#include <iostream>
#include "customError.h"
#include "RBTree.h"

using namespace std;
#define RED 1
#define BLACK 0

// *CONSTRUCTOR*
C_RBTree::C_RBTree() {
	p_root = NULL;
};

// *DESTRUCTOR*
C_RBTree::~C_RBTree() {
	p_root = P_deleteTree(p_root);
}


void C_RBTree::P_insertValue(int value) {
	try {
		if (p_root == NULL) {
			p_root = new Node();
			p_root->data = value;
			p_root->lchild = NULL;
			p_root->rchild = NULL;
			p_root->parent = NULL;
			p_root->color = BLACK;
		}
		else {
			p_insertValue(p_root, value);
		}
	}
	catch (MyException& e) {
		cout << e.what() << endl;
	}
}

bool P_findNode(int);
void P_printTree();
void P_sortTree();
Node* P_deleteTree(Node*);


Node* C_RBTree::p_insertValue(Node* n, int value) {
	if (P_findNode(value) && n != p_root) {
		throw MyException("WARNING: Value provided already exists. No value was inserted.");
	}
	else {
		if (n == NULL) {
			n = new Node();
			n->data = value;
			n->lchild = NULL;
			n->rchild = NULL;
			n->parent = p_setParent(p_root, value);
			n->color = RED;
		}
		else if (value > n->data) {
			n->lchild = p_insertValue(n->lchild, value);
		}
		else if (value < n->data) {
			n->rchild = p_insertValue(n->rchild, value);
		}
		p_updateColors(n);
		return n;
	}
}

Node* C_RBTree::p_setParent(Node* n, int value) {
	if (value > n->data) {
		if (value == n->lchild->data || value == n->rchild->data) {
			return n;
		}
		n->lchild = p_setParent(n->lchild, value);
	}
	else {
		if (value == n->rchild->data || value == n->lchild->data) {
			return n;
		}
		n->rchild = p_setParent(n->rchild, value);
	}
}

void C_RBTree::p_updateColors(Node* n) {
	Node* grandparent = n->parent->parent;
	Node* currentParent = n->parent;
	if (!(n == p_root)) {
		// IF parent and uncle are BOTH red.
		if (grandparent->lchild->color == RED && grandparent->rchild->color == RED) {
			grandparent->lchild->color = BLACK;
			grandparent->rchild->color = BLACK;
			grandparent->color = RED;
			if (grandparent == p_root) {
				grandparent->color = BLACK;
			}
			p_updateColors(n->parent);
		}
		else if (p_isBlack(grandparent->lchild) && p_isRed(grandparent->rchild)) {
			if (grandparent->rchild = currentParent) {
				if (currentParent->rchild = n) {
					grandparent->color = RED;
					currentParent->color = BLACK;
					p_rotateLeft(grandparent, currentParent, n);
				}
				if (currentParent->lchild = n) {
					grandparent->color = RED;
					currentParent->color = BLACK;
					p_rotateRight(grandparent, currentParent, n);
					p_rotateLeft(grandparent, currentParent, n);
				}
			}
			p_updateColors(n->parent);
		}
		else if (p_isBlack(grandparent->rchild) && p_isRed(grandparent->lchild)) {
			if (grandparent->lchild = currentParent) {
				if (currentParent->lchild = n) {
					grandparent->color = RED;
					currentParent->color = BLACK;
					p_rotateRight(grandparent, currentParent, n);
				}
				if (currentParent->rchild = n) {
					grandparent->color = RED;
					currentParent->color = BLACK;
					p_rotateLeft(grandparent, currentParent, n);
					p_rotateRight(grandparent, currentParent, n);
				}
			}
			p_updateColors(n->parent);
		}
			// IF parent and uncle are red and black (in either order).
	}
}

bool C_RBTree::p_isBlack(Node* n) {
	if (n == NULL || n->color == BLACK) {
		return true;
	}
	return false;
}

bool C_RBTree::p_isRed(Node* n) {
	if (n->color == RED) {
		return true;
	}
	return false;
}

void C_RBTree::p_rotateLeft(Node* gp, Node* p, Node* rc) {
	if (gp != NULL) {
		gp->rchild = rc;
	}
	else {
		p_root = rc;
	}
	p->rchild = rc->lchild;
	rc->lchild = p;
}
void C_RBTree::p_rotateRight(Node* gp, Node* p, Node* lc) {
	if (gp != NULL) {
		gp->lchild = lc;
	}
	else {
		p_root = lc;
	}
	p->lchild = lc->rchild;
	lc->rchild = p;
}
