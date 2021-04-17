#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "RBTree.h"
#include "customError.h"

#define RED 1
#define BLACK 0
#define NIL -1

/* CONSTRUCTOR */
C_RBTree::C_RBTree() {
    p_root = NULL;
}

/* DESTRUCTOR */
C_RBTree::~C_RBTree() {
    p_root = P_deleteTree();
}

/*
**    Author: Levi
**    Function Purpose:
**      Calls the delete tree function.
**
**    Function Output: calls the delete tree function.
**    Side Effects:
*/
Node* C_RBTree::P_deleteTree() {
    p_deleteTree(p_root);
    p_root = NULL;
    return p_root;
}

/*
**    Author: Levi
**    Function Purpose:
**      Deletes the entire tree.
**
**    Function Output: Deleted tree.
**    Side Effects:
*/
void C_RBTree::p_deleteTree(Node* &n) {
    if (n == NULL) {
        return;
     }
    p_deleteTree(n->lchild);
    p_deleteTree(n->rchild);
    delete n;

}

/*
**    Author: Levi
**    Function Purpose:
**      Calls printing function and throws exception if there is an empty tree.
**
**    Function Output: Bool of whether a given node is black.
**    Side Effects:
*/
void C_RBTree::P_printTree() {
    if (p_root == NULL) {
        throw MyException("ERROR: Empty tree cannot be printed.");
    }
    else {
        p_printTree(p_root, 0);
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      Prints a formatted tree.
**
**    Function Output: Printed tree.
**    Side Effects:
*/
void C_RBTree::p_printTree(Node* n, int indent) {
    if (n->data == NIL || n == NULL) {
        return;
    }
    else {
        this->p_printTree(n->lchild, indent + 4);
        if (indent) {
            cout << setw(indent) << " ";
        }
        cout << n->data;
        if (p_isRed(n)) {
            cout << "(Red)  " << endl;
        }
        else {
            cout << "(Black)" << endl;
        }
        this->p_printTree(n->rchild, indent + 4);
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      Inserts a root or calls a function to insert a node elsewhere before color correcting it. Also does some error checking.
**
**    Function Output: Calls insertion functions, deals with errors, and calls for color correcting.
**    Side Effects:
*/
void C_RBTree::P_insertValue(int val) {
    try {
        if (p_root == NULL) {
            p_root = new Node(val);
            p_root->data = val;
            p_root->lchild = p_createNILNode(p_root);
            p_root->rchild = p_createNILNode(p_root);
            p_root->color = BLACK;
            p_root->parent = NULL;
        }
        else {
            Node* startCorrection = p_insertValue(NULL, p_root, val);
            p_colorCorrection(p_recentInsert);
        }
    }
    catch (MyException& e) {
        cout << e.what() << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      Resolves color conflicts and any violations of the rules stated in the powerpoints. Deals with the responsibility
**      of determining what case a tree is in as opposed to bearing the responsibility of fixing these cases (that is given
**      to the functions that are called within).
**
**    Function Output: Solves rules violations
**    Side Effects:
*/
void C_RBTree::p_colorCorrection(Node* n) {
    // If we aren't the root and we are violating the "red nodes may not have red children" relationship, then we
    // begin to recolor. The reason for n!=p_root is because rotating on root runs the issue of no parents existing
    // outside of NULL, and no parent of NULL exists either, so no grandparent, and finally, no uncle of p_root exists
    // either because grandparent is non-existent.
    while (n != p_root && p_isRed(n) && p_isRed(n->parent))
    {
        Node* parent = n->parent;
        Node* grandparent = n->parent->parent;

        // Deals with left side of tree.
        if (parent == grandparent->lchild)
        {
            Node* uncle = grandparent->rchild;
            if (uncle->color == RED)
            {
                // Red parent and red uncle case
                n = p_RRInsertion(parent, uncle, grandparent);
            }

            else
            {
                // red parent and black uncle case
                n = p_RBInsertion(n, parent, uncle, grandparent);
            }
        }

        else
        {
            Node* uncle = grandparent->lchild;
            if (uncle->color == RED)
            {
                n = p_RRInsertion(parent, uncle, grandparent);
            }
            else
            {
                n = p_RBInsertion(n, parent, uncle, grandparent);
            }
        }
    }
    p_root->color = BLACK;
}


/*
**    Author: Levi
**    Function Purpose:
**      Left rotation based on the BST left rotation, but this iteration has more conditions. The former function for rotating CANNOT be reliably used here
**      due to the former having a reliable vine structure, while this iteration cannot rely on any given format with consistency.
**
**    Function Output: Rotates nodes left
**    Side Effects:
*/
void C_RBTree::p_rotateLeft(Node* n)
{
    Node* rc = n->rchild;   // rc is an abbreviation for rightchild. It is not rchild to avoid any confusion with the Node property of rchild.
    n->rchild = rc->lchild;

    // Although it isn't always necessary, this must be apart of the function to prevent some rather persistent nullptr issues.
    if (n->rchild->data != NIL)
        n->rchild->parent = n;

    rc->parent = n->parent;
    if (n->parent == NULL) {
        p_root = rc;
    }
    else if (n == n->parent->lchild) {
        n->parent->lchild = rc;
    }
    else {
        n->parent->rchild = rc;
    }
    rc->lchild = n;
    n->parent = rc;
}

/*
**    Author: Levi
**    Function Purpose:
**      Right rotation based on the BST right rotation, but this iteration has more conditions. The former function for rotating CANNOT be reliably used here
**      due to the former having a reliable vine structure, while this iteration cannot rely on any given format with consistency.
**
**    Function Output: Rotates nodes right
**    Side Effects:
*/
void C_RBTree::p_rotateRight(Node* n)
{
    Node* lc = n->lchild;   // lc is an abbreviation for leftchild. It is not lchild to avoid any confusion with the Node property of lchild
    n->lchild = lc->rchild;

    if (n->lchild->data != NIL) {
        n->lchild->parent = n;
    }

    lc->parent = n->parent;
    if (n->parent == NULL) {
        p_root = lc;
    }
    else if (n == n->parent->lchild) {
        n->parent->lchild = lc;
    }
    else {
        n->parent->rchild = lc;
        }
    lc->rchild = n;
    n->parent = lc;
}

/*
**    Author: Levi
**    Function Purpose:
**      Deals with the case of Red uncle Red parent, which is pretty much just a fancy recoloring as portrayed in the power point.
**
**    Function Output: Recolered nodes
**    Side Effects:
*/
Node* C_RBTree::p_RRInsertion(Node* parent, Node* uncle, Node* grandparent) {
    // In the case of Red parent and Red uncle, the grandparent should become red and
    // the parent and uncle become black, but if the grandparent is the root, we should
    // leave it black.
    if (grandparent != p_root) {
        grandparent->color = RED;
    }
    parent->color = BLACK;
    uncle->color = BLACK;
    // Since this case only requires the change of color to already existing nodes 
    // (e.g the uncle, parent, and possibly the grandparent), we can skip to the furthest
    // known good node up the tree (the grandparent in this case.
    return grandparent;
}

/*
**    Author: Levi
**    Function Purpose:
**      Red Parent but black uncle which is dealt with by rotating and recoloring when necessary.
**
**    Function Output: Rotates nodes right
**    Side Effects: Although i'm not 100% confident with the concepts behind pass by reference, the mutability of parameters
**      to remain changed is what fixed this function to work correctly
*/
Node* C_RBTree::p_RBInsertion(Node* child, Node* parent, Node* uncle, Node* grandparent) {
    if (parent == grandparent->lchild) {
        // If there isn't a vine between the child node, the parent node, and the grandparent node,
        // then we need to rotate once left and recolor before rotating right. If it is a vine, just 
        // preform the second part.
        if (child == parent->rchild)
        {
            p_rotateLeft(parent);
            child = parent;
            parent = child->parent;
        }
        int tmpColor = parent->color;
        parent->color = grandparent->color;
        grandparent->color = tmpColor;
        p_rotateRight(grandparent);
        return parent;
    }
    else {
        // If there isn't a vine between the child node, the parent node, and the grandparent node,
        // then we need to rotate once left and recolor before rotating right. If it is a vine, just 
        // preform the second part.
        if (child == parent->lchild)
        {
            p_rotateRight(parent);
            child = parent;
            parent = child->parent;
        }
        int tmpColor = parent->color;
        parent->color = grandparent->color;
        grandparent->color = tmpColor;
        p_rotateLeft(grandparent);
        return parent;
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      Inserts a red node with the given value.
**
**    Function Output: Recursive insertion of a new node with red coloring
**    Side Effects:
*/
Node* C_RBTree::p_insertValue(Node* prev, Node* n, int val) {
    if (n->data == NIL) {
        n->parent = prev;
        n->data = val;
        n->lchild = p_createNILNode(n);
        n->rchild = p_createNILNode(n);
        n->color = RED;
        p_recentInsert = n;
    }
    else if (val > n->data) {
        prev = n;
        n->lchild = p_insertValue(prev, n->lchild, val);
    }
    else if (val < n->data) {
        prev = n;
        n->rchild = p_insertValue(prev, n->rchild, val);
    }
    else {
        throw MyException("ERROR: Cannot insert a value that already exists.");
    }
    return n;
}

/*
**    Author: Levi
**    Function Purpose:
**      Public facing deletion function.
**
**    Function Output: Allows deletion of node to be called from main.
**    Side Effects:
*/
void C_RBTree::P_deleteNode(int delKey) {
    if (p_exists(delKey)) {
        p_deleteNode(delKey, p_findNode(delKey));
    }
    else {
        throw MyException("ERROR: Node to be deleted does not exists.");
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      Helps figure out whether or not any resolution of deleted black nodes is needed.
**
**    Function Output: Either enters resolution of deleted black node, or calls function to remove red node.
**    Side Effects:
*/
void C_RBTree::p_deleteNode(int delKey, Node* delNode) {
    Node* successor = p_findSuccessor(delNode);
    if (successor->color == RED) {
        p_RRDeletion(delNode, successor);
    }
    else {
        delNode->data = successor->data;
        successor->data = -1;
        p_deletionFixer(successor);
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      The brains behind what should be destroyed, what node should usurp the position of another. IF for some reason
**      this method is entered with already valid node structure (didn't delete a black node) or we are at root, then the
**      loop does not execute.
**
**    Function Output: Resolves deleted black node issues.
**    Side Effects:
*/
void C_RBTree::p_deletionFixer(Node* delNode) {
    Node* sibling;
    Node* parent = delNode->parent;
    Node* child = delNode;
    // As long as we aren't root and are a black node
    while (child != p_root && child->color == BLACK) {
        // Just helps find out siblings orientation and what will need to be fixed in relation
        if (child == parent->lchild) {
            sibling = parent->rchild;
            // Black red case
            if (sibling->color == RED) {
                sibling = p_BRDeletion(child, sibling, parent);
            }
            // Sibling's children Black Black case
            if (sibling->lchild->color == BLACK && sibling->rchild->color == BLACK) {
                sibling->color = RED;
                child = child->parent;
            }
            // Black Black case
            else {
                p_BBDeletion(child, sibling, parent);
                child = p_root;
            }
        }
        // Just helps find out siblings orientation and what will need to be fixed in relation
        else {
            sibling = parent->lchild;
            // Black red case
            if (sibling->color == RED) {
                sibling = p_BRDeletion(child, sibling, parent);
                sibling = child->parent->lchild;
            }
            // Sibling's children Black Black case
            if (sibling->lchild->color == BLACK && sibling->rchild->color == BLACK) {
                sibling->color = RED;
                child = child->parent;
            }
            // Black Black case
            else {
                p_BBDeletion(child, sibling, parent);
                child = p_root;
            }
        }
    }
    // if we make it to root, then turn it black. Also solves the issue of ending on an invalid red node.
    child->color = BLACK;
}

/*
**    Author: Levi
**    Function Purpose:
**      Probably the simplest case possible, in which a red node is deleted. Just swap the data...
**
**    Function Output: A destroyed red leaf node.
**    Side Effects:
*/
void C_RBTree::p_RRDeletion(Node* subject, Node* successor) {
    subject->data = successor->data;
    successor->rchild = NULL;
    successor->lchild = NULL;
    successor->data = NIL;
    successor->color = BLACK;
}

/*
**    Author: Levi
**    Function Purpose:
**      Fixes situations (like double black nodes) by rotating on the sibling ONLY if the sibling has its own children that
**      are not compliant to the situation needed. Either way, a recoloring followed by rotation on the parent node takes place.
**
**    Function Output: Return the node of a given value.
**    Side Effects:
*/
void C_RBTree::p_BBDeletion(Node* child, Node* sibling, Node* parent) {
    if (child == parent->lchild) {
        if (sibling->rchild->color == BLACK) {
            sibling->lchild->color = BLACK;
            sibling->color = RED;
            p_rotateRight(sibling);
            sibling = child->parent->rchild;
        }
        sibling->color = child->parent->color;
        child->parent->color = BLACK;
        sibling->rchild->color = BLACK;
        p_rotateLeft(child->parent);
    }
    else {
        if (sibling->lchild->color == BLACK) {
            sibling->rchild->color = BLACK;
            sibling->color = RED;
            p_rotateLeft(sibling);
            sibling = child->parent->lchild;
        }
        sibling->color = child->parent->color;
        child->parent->color = BLACK;
        sibling->lchild->color = BLACK;
        p_rotateRight(child->parent);
    }

}

/*
**    Author: Levi
**    Function Purpose:
**      Deals with Black Red deletion case, determining what side the deleted Node was on realative to the parent node,
**      before swapping the colors out for the sibling and parent (something that doesn't need to be conditionally under a specific
**      if else, as which side they are on is resolved in the function which calls this). Based on the booleans from the beginning, the
**      rotations on the parents are decided.
**
**    Function Output: Fixes deletion when a red sibling exists for a black child (deleted) node.
**    Side Effects:
*/
Node* C_RBTree::p_BRDeletion(Node* &child, Node* &sibling, Node* &parent) {
    bool isRight;
    if (sibling == parent->lchild) {
        isRight = true;
    }
    else {
        isRight = false;
    }
    sibling->color = BLACK;
    parent->color = RED;
    
    if (isRight) {
        p_rotateRight(parent);
        sibling = child->parent->lchild;
    }
    else {
        p_rotateLeft(parent);
        sibling = child->parent->rchild;
    }
    return sibling;

}

/*
**    Author: Levi
**    Function Purpose:
**      Finds the sucessor to a given node, whether that be itself, the only child it has, or the furthest left child in the right subtree,
**
**    Function Output: Returns the best successor, specifically to usurp a given Node's position during replacement.
**    Side Effects:
*/
Node* C_RBTree::p_findSuccessor(Node* parent) {
    if (parent->lchild->data != NIL && parent->rchild->data != NIL) {
        Node* traversal = parent->rchild;
        while (traversal->lchild->data != NIL) {
            traversal = traversal->lchild;
        }
        return traversal;
    }
    else if (parent->lchild->data == NIL && parent->rchild->data == NIL) {
        return parent;
    }
    else if (parent->lchild->data != NIL) {
        return parent->lchild;
    }
    else {
        return parent->rchild;
    }
}

/*
**    Author: Levi
**    Function Purpose:
**      Finds out if a given node exists, and then returns a boolean of whether it does or not.
**
**    Function Output: Returns a boolean determining existence of a node based on value.
**    Side Effects:
*/
bool C_RBTree::p_exists(int val) {
    Node* traversal = p_root;
    while (traversal->lchild != NULL && traversal->rchild != NULL) {
        if (traversal->data > val) {
            traversal = traversal->rchild;
        }
        else if (traversal->data < val) {
            traversal = traversal->lchild;
        }
        else {
            return true;
        }
    }
    return false;
}

/*
**    Author: Levi
**    Function Purpose:
**      Finds a given node in the tree and returns it. Used in tandem with the exists function.
**
**    Function Output: Return the node of a given value.
**    Side Effects:
*/
Node* C_RBTree::p_findNode(int val) {
    Node* traversal = p_root;
    while (traversal->data != val) {
        if (traversal->data > val) {
            traversal = traversal->rchild;
        }
        else {
            traversal = traversal->lchild;
        }
    }
    return traversal;
}

/*
**    Author: Levi
**    Function Purpose:
**      Instead of leaf nodes pointing to nodes, this function makes NIL nodes, which possess a value of -1,
**      are black, and point to NULL. This helps reinforce the rule of NULL nodes pointed to by leafs are black,
**      and provide a workable node for comparison of color, as opposed to trying to work with NULL as a value.
**
**    Function Output: Creates a new NIL node
**    Side Effects:
*/
Node* C_RBTree::p_createNILNode(Node* parent) {
    Node* NILNode = new Node(NIL);
    NILNode->parent = parent;
    NILNode->lchild = NULL;
    NILNode->rchild = NULL;
    NILNode->color = BLACK;
    return NILNode;
}

/*
**    Author: Levi
**    Function Purpose:
**      Determines whether a node is black.
**
**    Function Output: Bool of whether a given node is black.
**    Side Effects:
*/
bool C_RBTree::p_isBlack(Node* n) {
    if (n->color == BLACK) {
        return true;
    }
    return false;
}

/*
**    Author: Levi
**    Function Purpose:
**      Determines whether a node is red.
**
**    Function Output: Bool of whether a given node is red.
**    Side Effects:
*/
bool C_RBTree::p_isRed(Node* n) {
    if (n->color == RED) {
        return true;
    }
    return false;
}
