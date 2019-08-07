#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include <vector>
#include <string>
using namespace std;

//clear if unused: *pNext

enum Color {RED, BLACK};

struct Node {
    int data;
    bool color;
    Node *pChild[2], *pParent, *pSibling, *pNext; // pNext NAO FUNCIONA, IRRELEVANTE?
    Node(int x):data(x) {
        pChild[0] = pChild[1] = pParent = pSibling = pNext = nullptr; // pNext NAO FUNCIONA, IRRELEVANTE?
    }
};

class BST {
protected:
    Node *pRoot;
public:
    //funcs to work with initialization list of ints
    BST(initializer_list<int> values);
    template<typename ...Ts>
    BST(Ts... ts);

    BST():pRoot(nullptr) {} //initialize null pRoot
    void RB_remove(int);
    void RB_insert(int &);
    
    //not used in script
    bool find(int x) {
        Node **p;
        return find(x, p);
    }
    
    //void insert(int x) {
    //    Node **p;
    //    if (!find(x, p)) {
    //        *p = new Node(x);
    //    }
    //}
    void insert(int x) {
        Node **p;
        if (!find(x, p)) {
            *p = new Node(x);
        }
        cout << "*p " << *p << " " << &x << " " << x << endl; //<<
        RB_insert(pRoot, *p);
        RB_insert_fixup(pRoot, *p);
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

private:
    void process() {} // irrelevant
    template <typename... Ts> // irrelevant
    void process(int t, Ts... ts); // irrelevant
    
    void left_rotate(Node *&, Node *&);
    void right_rotate(Node *&, Node *&);
    Node* RB_insert(Node *, Node *);
    void RB_insert_fixup(Node *&, Node *&);
    void RB_delete(Node *);
    void RB_delete_fixup(Node *);
    
    bool find(int x, Node **&p) {
        p = &pRoot;
        cout << "root,**p&: " << &pRoot << " "<< pRoot << " "<< p << " "<< *p  << endl; //**p
        while(*p) {
            if ((*p)->data==x) return true;
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
    Node *get_sibling(Node *X) {
        if (X->pParent)
            return X->pParent->pChild[X == X->pParent->pChild[0]]; // if compares X.data to parent.data, doesnt work beacause in some cases the remove func swaps X to sibling position. Would have to change code in other parts.
    }
};
/* END OF HEADER */

/* ROTATION FUNCTIONS*/
void BST::left_rotate(Node *&pRoot, Node *&X) {
    Node *Y = X->pChild[1];

    //nova relacao de y.left (beta no livro)
    X->pChild[1] = Y->pChild[0];
    if (Y->pChild[0]) // mudei para = livro
        X->pChild[1]->pParent = X; // book seems wrong in this case, as Y->left would be X. See penultimate row of this function

    // nova relacao de X.pParent
    Y->pParent = X->pParent;
    if (!X->pParent) // if X was root, Y will be the new root
        pRoot = Y;
    else if (X == X->pParent->pChild[0])
        X->pParent->pChild[0] = Y;
    else
        X->pParent->pChild[1] = Y;

    //nova relacao entre x e y
    Y->pChild[0] = X;
    X->pParent = Y;
}

void BST::right_rotate(Node *&pRoot, Node *&X) {
    Node *Y = X->pChild[0];

    //nova relacao de y.right (beta no livro)
    X->pChild[0] = Y->pChild[1];
    if (Y->pChild[1])
        X->pChild[0]->pParent = X;

    // nova relacao de X.pParent
    Y->pParent = X->pParent;
    if (!X->pParent)
        pRoot = Y;
    else if (X == X->pParent->pChild[0])
        X->pParent->pChild[0] = Y;
    else
        X->pParent->pChild[1] = Y;

    //nova relacao entre x e y
    Y->pChild[1] = X;
    X->pParent = Y;
}

/* INSERTION FUNCTIONS*/
//falta o caso quando X já existe
Node* BST::RB_insert(Node* subTree, Node *X) {
    /* X será a nova raiz, se tree estiver vazia */
    if (!subTree) return X;

    /* desce a árvore até encontrar um ponto para encaixar */
    else {
        if (X->data < subTree->data) 	{
            subTree->pChild[0] = RB_insert(subTree->pChild[0], X);
            subTree->pChild[0]->pParent = subTree;
        }
        else if (X->data > subTree->data) {
            subTree->pChild[1] = RB_insert(subTree->pChild[1], X);
            subTree->pChild[1]->pParent = subTree;
        }
    }
    /* return the (unchanged) node pointer */
    return subTree;
}

// This function fixes violations caused by BST insertion
void BST::RB_insert_fixup(Node *&subTree, Node *&Z) {
    Node *Z_pParent = nullptr;
    Node *Z_pGrParent = nullptr;
    Z->color = RED;
    //Z->pParent->color == BLACK // temp, insert bugged
    while ((Z != subTree) && (Z->color != BLACK) && (Z->pParent->color == RED)) {
        Z_pParent = Z->pParent;
        Z_pGrParent = Z->pParent->pParent;

        /* Z.pParent is at left */
        if (Z_pParent == Z_pGrParent->pChild[0]) {
            Node *Y = Z_pGrParent->pChild[1];

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if (Y  && Y->color == RED) {
                Z_pParent->color = BLACK;
                Y->color = BLACK;
                Z_pGrParent->color = RED;
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
                swap(Z_pParent->color, Z_pGrParent->color);
                Z = Z_pParent;
            }
        }

        /* Z.p is at right */
        else{
            Node *Y = Z_pGrParent->pChild[0];

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if (Y && Y->color == RED) {
                Z_pParent->color = BLACK;
                Y->color = BLACK;
                Z_pGrParent->color = RED;
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
                swap(Z_pParent->color, Z_pGrParent->color);
                Z = Z_pParent;
            }
        }
    }

    subTree->color = BLACK; // needs to assure that root is black
}

void BST::RB_insert(int &data) {
    cout << data << " "<< " "<< &data << endl;
    Node *Z = new Node(data);
    cout << "Z: " << Z << " " << " " << Z << endl;
    Node **p;
    //if (!find(x, p)) {
    //    *p = new Node(x);
    //}
    // insert node
    pRoot = RB_insert(pRoot, Z);

    // fixup the tree after insertion
    RB_insert_fixup(pRoot, Z);
}

/* DELETION FUNCTIONS*/
// deletes the given node
void BST::RB_delete(Node *Z) {
    Node *pParent = Z->pParent;
    Node *pSibling = get_sibling(Z);
    Node *X = remove(Z); // X is succesor of Z, as in the book

    // True when X and Z are both black
    bool doubly_black = ((Z->color == BLACK) && (!X || X->color == BLACK));

    if (!X) {
        // X is NULL therefore Z is leaf
        if (Z == pRoot) {
            pRoot = nullptr;
        }
        else {
            if (doubly_black) {
                // X and Z both black
                // Z is leaf, fix double black at Z
                RB_delete_fixup(Z);
            }
            else {
                // X or Z is red
                if (Z->pSibling)
                    // sibling is not null, make it red"
                    Z->pSibling->color = RED;
            }

            // delete Z from the tree
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
        // Z has 1 child
        if (Z == pRoot) {
            // Z is root, assign the value of X to Z, and delete X
            Z->data = X->data;
            Z->pChild[0] = Z->pChild[1] = nullptr;
            delete X;
        }
        else {
            // Detach Z from tree and move X up
            if (Z == Z->pParent->pChild[0]) {
                pParent->pChild[0] = X;
            }
            else {
                pParent->pChild[1] = X;
            }
            delete Z;
            X->pParent = pParent;
            if (doubly_black) {
                // X and Z both black, fix double black at X
                RB_delete_fixup(X);
            }
            else {
                // X or Z red, color X black
                X->color = BLACK;
            }
        }
        return;
    }

    // Z has 2 children, swap values with succesor and recurse
    swap(X->data, Z->data);
    RB_delete(X);
}

void BST::RB_delete_fixup(Node *X) {
    if (X == pRoot) return;

    Node *pParent = X->pParent;
    Node *pSibling = get_sibling(X);

    if (!pSibling) {
        // No sibiling, double black pushed up
        RB_delete_fixup(pParent);
    }
    else {
        if (pSibling->color == RED) {
            // Sibling red
            pParent->color = RED;
            pSibling->color = BLACK;
            if (pSibling == pSibling->pParent->pChild[0]) {
                // left case
                right_rotate(pRoot, pParent);
            }
            else {
                // right case
                left_rotate(pRoot, pParent);
            }
            RB_delete_fixup(X);
        }
    else {
        // Sibling black
        if (pSibling->pChild[0]->color == RED || pSibling->pChild[1]->color == RED) {
        // at least 1 red children
            if (pSibling->pChild[0] && pSibling->pChild[0]->color == RED) {
                if (pSibling == pSibling->pParent->pChild[0]) {
                    // left left
                    pSibling->pChild[0]->color = pSibling->color;
                    pSibling->color = pParent->color;
                    right_rotate(pRoot, pParent);
                }
                else {
                    // right left
                    pSibling->pChild[0]->color = pParent->color;
                    right_rotate(pRoot, pSibling);
                    left_rotate(pRoot, pParent);
                }
        }
        else {
            if (pSibling == pSibling->pParent->pChild[0]) {
                // left right
                pSibling->pChild[1]->color = pParent->color;
                left_rotate(pRoot, pSibling);
                right_rotate(pRoot, pParent);
            }
            else {
                // right right
                pSibling->pChild[1]->color = pSibling->color;
                pSibling->color = pParent->color;
                left_rotate(pRoot, pParent);
            }
        }
        pParent->color = BLACK;
        }
        else {
            // 2 black children
            pSibling->color = RED;
            if (pParent->color == BLACK)
                RB_delete_fixup(pParent);
            else
                pParent->color = BLACK;
        }
    }
    }
}

//template <typename... Ts>//void BST::process(int t, Ts... ts) {//    RB_insert(t);//    this->process(ts...);//}
//initialize null pRoot
BST::BST(initializer_list<int> values):pRoot(nullptr) {
    for (auto val: values) {this->RB_insert(val);   }  }

int main() {
    BST rbt({41, 38, 31, 12, 19, 8});

    cout << "\n\narvore gerada\n";
    rbt.print();

    rbt.remove(19);
    cout << "\n\narvore apos delecao\n";
    rbt.print();
    return 0;
}
