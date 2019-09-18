#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <initializer_list>

using namespace std;

/* 
 * AS MAIN REFERENCES, I AM USING:
 * BOOK: "Introduction to Algorithms", cap 13 (CORMEN, Thomas et al), third edition.
 * Slide L3 from course at Standford which is being used by the teacher.
 * * Link: http://web.stanford.edu/class/archive/cs/cs161/cs161.1178/
 * I tried to make RBT to inherit from BST or use it as a friend, but its not working. So I had to change BST directly.
 * RBtrees might also be tested at the following links:
 * * cs.armstrong.edu/liang/animation/web/RBTree.html 
 * * https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
 */
 
struct Node {
    int data;
    bool pColor;
    Node *pChild[2], *pParent, *pSibling; 
    Node(int x):data(x) {
        pChild[0] = pChild[1] = pParent = pSibling  = nullptr;
    }
};

class BST {
    friend class RBT;
protected:
    Node *pRoot;
public:    
    BST():pRoot(nullptr) {} //initialize null pRoot

    bool find(int x) {
        Node **p;
        return find(x, p);
    }
    
    void insert(int x) {
        Node **p;
        if (!find(x, p)) {
            *p = new Node(x);
        }
    }

    void remove(int x) {
        Node **p;
        if(find(x, p))
            //remove(*p);
            RB_delete(*p);
    }

    void print() {
        print(pRoot);
        cout << endl;
    }

    void RB_insert(int &);
private:    
    //function declarations. Prototypes are located after header.
    void left_rotate(Node *&, Node *&);
    void right_rotate(Node *&, Node *&);
    Node* RB_find_or_insert(Node *, Node *);
    void RB_insert_fixup(Node *&, Node *&);
    void RB_delete(Node *);
    void RB_delete_fixup(Node *);
    
    bool find(int x, Node **&p) {
        p = &pRoot;
        //cout << "root,**p&: " << &pRoot << " "<< pRoot << " "<< p << " "<< *p  << endl; //**p
        while(*p) {
            if ((*p)->data==x) return true;
            p = &((*p)->pChild[(*p)->data < x]);
        }
        return false;
    }
    
    bool RB_find(int x, Node **&p, Node **&parent) {
        p = &pRoot;
        while(*p) {
            if ((*p)->data==x) return true;
            parent = p;
            p = &((*p)->pChild[(*p)->data < x]);
        }
        return false;
    }

    // get pointer of succesor
    //void remove(Node *&p) {
    Node *remove(Node *&p) {
        if (!p->pChild[0] || !p->pChild[1])
            //p = p->pChild[p->pChild[1]!=nullptr];
            return p->pChild[p->pChild[1]!=nullptr];
        else {
            Node **succesor = &(p->pChild[1]);
            find_min(succesor);
            //p->data = (*succesor)->data;
            //remove(*succesor);
            return *succesor;
        }
    }

    void find_min(Node **&p) {
        while((*p)->pChild[0]){
            p = &((*p)->pChild[0]);
        }
    }

    void print(Node *p, int indent=0) {
        if (p) {
            print(p->pChild[1], indent+6);
            cout << setw(indent) << ' ';
            cout<< p->data <<endl;
            print(p->pChild[0], indent+6);
        }
    }

    // get pointer of sibling
    Node *get_sibling(Node *X);
};

class RBT: public BST{
public:
    template<typename ...Ts>
    RBT(Ts... ts);
    
    Node *get_sibling(Node *X);
private:
    void process() {}
    template <typename... Ts>
    void process(int t, Ts... ts);
};

enum COLOR {cREd, cBlack};

/* END OF HEADER */

/* TRYING TO USE INHERITANCE OR FRIEND OF CLASS */
Node* RBT::get_sibling(Node *X) {
    if (X->pParent)
        return X->pParent->pChild[X == X->pParent->pChild[0]]; // if compares X.data to parent.data, doesnt work beacause in some cases the remove func swaps X to sibling position. Would have to change code in other parts.
}
// I should be using RBT to inherit from BST, but its not working
Node* BST::get_sibling(Node *X) {
    if (X->pParent)
        return X->pParent->pChild[X == X->pParent->pChild[0]]; // if compares X.data to parent.data, doesnt work beacause in some cases the remove func swaps X to sibling position. Would have to change code in other parts.
}
    
/* ROTATION FUNCTIONS - see book pg 313 */
void BST::left_rotate(Node *&pRoot, Node *&X) {
    Node *Y = X->pChild[1]; // set X

    //new relationship of y.left (beta in the book)
    X->pChild[1] = Y->pChild[0];   // turn ys left subtree into xs right subtree
    if (Y->pChild[0]) // Y as in the book
        X->pChild[1]->pParent = X; // book seems wrong in this case, as Y->left would be X. See penultimate row of this function

    Y->pParent = X->pParent; // link xs parent to y
    if (!X->pParent) // if X was root, Y will be the new root
        pRoot = Y;
    else if (X == X->pParent->pChild[0])
        X->pParent->pChild[0] = Y;
    else
        X->pParent->pChild[1] = Y;

    //new relationship for x and y
    Y->pChild[0] = X;       // put x on ys left
    X->pParent = Y;
}

void BST::right_rotate(Node *&pRoot, Node *&X) {
    Node *Y = X->pChild[0]; // set X, everything below is analogous to previous rotation

    //new relationship of y.right (beta in the book)
    X->pChild[0] = Y->pChild[1];
    if (Y->pChild[1])
        X->pChild[0]->pParent = X;

    // new relationship of X.pParent
    Y->pParent = X->pParent;
    if (!X->pParent)
        pRoot = Y;
    else if (X == X->pParent->pChild[0])
        X->pParent->pChild[0] = Y;
    else
        X->pParent->pChild[1] = Y;

    //new relationship for x and y
    Y->pChild[1] = X;
    X->pParent = Y;
}

/* INSERTION FUNCTIONS*/
void BST::RB_insert(int &data) {
    //cout << data << " "<< " "<< &data << endl;
    Node *Z = new Node(data);
    //cout << "Z: " << Z << " " << " " << Z << endl;
    Node **p;
    // insert node
    pRoot = RB_find_or_insert(pRoot, Z);
    // fix color violations
    if (pRoot)
        RB_insert_fixup(pRoot, Z);
}

// see book pg 315
Node* BST::RB_find_or_insert(Node* subTree, Node *X) {
    /* X is new root if root is null */
    if (!subTree) return X;

    /* explores the tree till finding a new leaf to append to the tree */
    else {
        if (X->data < subTree->data) 	{
            subTree->pChild[0] = RB_find_or_insert(subTree->pChild[0], X);
            subTree->pChild[0]->pParent = subTree;
        }
        else if (X->data > subTree->data) {
            subTree->pChild[1] = RB_find_or_insert(subTree->pChild[1], X);
            subTree->pChild[1]->pParent = subTree;
        }
        else {
            return X; //if X already exists... are bugs possible here?
        }
    }
    return subTree;
}

// fix color errors of insertion - see book pg 316
void BST::RB_insert_fixup(Node *&subTree, Node *&Z) {
    Node *Z_pParent = nullptr;
    Node *Z_pGrParent = nullptr;
    Z->pColor = cREd;
    //Z->pParent->color == cBlack
    while ((Z != subTree) && (Z->pColor != cBlack) && (Z->pParent->pColor == cREd)) {
        Z_pParent = Z->pParent;
        Z_pGrParent = Z->pParent->pParent;

        /* Z.pParent is at left */
        if (Z_pParent == Z_pGrParent->pChild[0]) {
            Node *Y = Z_pGrParent->pChild[1];

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if (Y && Y->pColor == cREd) {
                Z_pParent->pColor = cBlack;
                Y->pColor = cBlack;
                Z_pGrParent->pColor = cREd;
                Z = Z_pGrParent;
            }
            else{
                /* Z is at right: left-rotation of Z.p, then right-rotation of Z.p.p */
                if (Z == Z_pParent->pChild[1]) {
                    left_rotate(subTree, Z_pParent);
                    Z = Z_pParent; // this is necessary for the other rotation to work correctly. Otherwise Z.p.p would be actually the grand_grand_pParent.
                    Z_pParent = Z->pParent; // redefine Z_p=Z.p, after rotation.
                }
                /* Z is at left: only right-rotation of Z.p.p */
                right_rotate(subTree, Z_pGrParent); // apos rotacao, vai colocar pParent acima de grandpParent
                swap(Z_pParent->pColor, Z_pGrParent->pColor);
                Z = Z_pParent;
            }
        }

        /* Z.p is at right */
        else{
            Node *Y = Z_pGrParent->pChild[0];

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if (Y && Y->pColor == cREd) {
                Z_pParent->pColor = cBlack;
                Y->pColor = cBlack;
                Z_pGrParent->pColor = cREd;
                Z = Z_pGrParent;
            }
            else{
                /* Z is at left: right-rotation of Z.p, then left-rotation of Z.p.p */
                if (Z == Z_pParent->pChild[0]) {
                    right_rotate(subTree, Z_pParent);
                    Z = Z_pParent; // this is necessary for the other rotation to work correctly. Otherwise Z.p.p would be actually the grand_grand_pParent.
                    Z_pParent = Z->pParent; // redefine Z_p=Z.p, after rotation.
                }
                /* Z is at right: only left-rotation of Z.p.p */
                left_rotate(subTree, Z_pGrParent); // apos rotacao, vai colocar pParent acima de grandpParent
                swap(Z_pParent->pColor, Z_pGrParent->pColor);
                Z = Z_pParent;
            }
        }
    }

    subTree->pColor = cBlack; // needs to assure that root is black
}

/* DELETION FUNCTIONS - see book pg 324*/
void BST::RB_delete(Node *Z) {
    Node *pParent = Z->pParent;
    Node *pSibling = get_sibling(Z);
    Node *X = remove(Z); // X is succesor of Z, as in the book

    // doubly_black = X && Z both black. See book pg 326
    bool doubly_black = ((Z->pColor == cBlack) && (!X || X->pColor == cBlack));

    if (!X) {
        // if X is null
        if (Z == pRoot) {
            pRoot = nullptr;
        }
        else {
            if (doubly_black) {
                // X and Z are black
                // Z has no child, fix colors
                RB_delete_fixup(Z);
            }
            else {
                // X or Z are red
                if (Z->pSibling)
                    // Z has sibling, recolor
                    Z->pSibling->pColor = cREd;
            }

            // remove Z
            if (Z == Z->pParent->pChild[0]) {
                pParent->pChild[0] = nullptr;
            }
            else {
                pParent->pChild[1] = nullptr;
            }
        }
        delete Z; return;
    }

    if (!Z->pChild[0] || !Z->pChild[1]) {
        // only one child
        if (Z == pRoot) {
            // Z is tree root, Z=X, remove X
            Z->data = X->data;
            Z->pChild[0] = Z->pChild[1] = nullptr;
            delete X;
        }
        else {
            // Z pruned, move X up
            if (Z == Z->pParent->pChild[0]) {
                pParent->pChild[0] = X;
            }
            else {
                pParent->pChild[1] = X;
            }
            delete Z;
            X->pParent = pParent;
            if (doubly_black) {
                // X and Z are black, fix colors
                RB_delete_fixup(X);
            }
            else {
                // X or Z red, Xcolor then black
                X->pColor = cBlack;
            }
        }
        return;
    }

    // Z has two child, swap values, do recursion up
    swap(X->data, Z->data);
    RB_delete(X);
}

void BST::RB_delete_fixup(Node *X) {
    if (X == pRoot) return;

    Node *pParent = X->pParent;
    Node *pSibling = get_sibling(X);

    if (!pSibling) {

        RB_delete_fixup(pParent);
    }
    else {
        if (pSibling->pColor == cREd) {

            pParent->pColor = cREd;
            pSibling->pColor = cBlack;
            if (pSibling == pSibling->pParent->pChild[0]) {

                right_rotate(pRoot, pParent);
            }
            else {

                left_rotate(pRoot, pParent);
            }
            RB_delete_fixup(X);
        }
    else {

        if (pSibling->pChild[0]->pColor == cREd || pSibling->pChild[1]->pColor == cREd) {

            if (pSibling->pChild[0] && pSibling->pChild[0]->pColor == cREd) {
                if (pSibling == pSibling->pParent->pChild[0]) {

                    pSibling->pChild[0]->pColor = pSibling->pColor;
                    pSibling->pColor = pParent->pColor;
                    right_rotate(pRoot, pParent);
                }
                else {

                    pSibling->pChild[0]->pColor = pParent->pColor;
                    right_rotate(pRoot, pSibling);
                    left_rotate(pRoot, pParent);
                }
        }
        else {
            if (pSibling == pSibling->pParent->pChild[0]) {

                pSibling->pChild[1]->pColor = pParent->pColor;
                left_rotate(pRoot, pSibling);
                right_rotate(pRoot, pParent);
            }
            else {

                pSibling->pChild[1]->pColor = pSibling->pColor;
                pSibling->pColor = pParent->pColor;
                left_rotate(pRoot, pParent);
            }
        }
        pParent->pColor = cBlack;
        }
        else {

            pSibling->pColor = cREd;
            if (pParent->pColor == cBlack)
                RB_delete_fixup(pParent);
            else
                pParent->pColor = cBlack;
        }
    }
    }
}

template<typename ...Ts>
RBT::RBT(Ts... ts): BST() {
    process(ts...);
}
template <typename... Ts>
void RBT::process(int t, Ts... ts) {
    BST::RB_insert(t);
    this->process(ts...);
}

int main() {
    RBT rbt(41, 38, 31, 12, 19, 8);
    cout << "\n\ngenerated tree -- read from left to right: root is at left, leaves are at right\n";
    rbt.print();

    rbt.remove(19);
    cout << "\n\ntree after deletion -- read from left to right: root is at left, leaves are at right\n";
    rbt.print();
    
    //2nd test
    RBT rbt_long(1, 4, 20, 19, 15, 100, 2, 8, 75, 23, 45, 7, 3, 14, 33);
    cout << "\n\nsecond generated tree -- read from left to right: root is at left, leaves are at right\n";
    rbt_long.print();   
    
    return 0;
}