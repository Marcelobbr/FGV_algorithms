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

class RBTree {
protected:
    Node *pRoot;
public:
    //funcs to work with initialization list of ints
    RBTree(initializer_list<int> values);
    void push_back(int x);
    template<typename ...Ts>
    RBTree(Ts... ts);

    RBTree() { pRoot = nullptr; }
    void RB_remove(int);
    void RB_insert(int &);

    //bool find(int x) {
    //    Node **p;
    //    return find(x, p);
    //}
    
    void insert(int x) {
        Node **p;
        if (!find(x, p)) {
            *p = new Node(x);
        }
        cout << "*p " << *p << " " << &x << " " << x << endl; //<<
        RB_insert_fixup(*&pRoot, **&p);
        //RB_insert(pRoot, *p);
    }
    void remove(int x) {
        Node **p;
        if(find(x, p))
            remove(*p);
    }

    void print() {
        print(pRoot);
        cout << endl;
    }
    //estou usando esse search em vez do find acima para deletar
    Node *search(int X) {
        Node *temp = pRoot;
        while (temp != nullptr) {
            if (X < temp->data) {
                if (temp->pChild[0] == nullptr) break;
                else temp = temp->pChild[0];
            }
            else if (X == temp->data) break;
            else {
                if (temp->pChild[1] == nullptr) break;
                else temp = temp->pChild[1];
            }
        }
        return temp;
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

    void remove(Node *&p) {
        if (!p->pChild[0] || !p->pChild[1])
            p = p->pChild[p->pChild[1]!=nullptr];
        else {
            Node **succesor = &(p->pChild[1]);
            find_min(succesor);
            p->data = (*succesor)->data;
            remove(*succesor);
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
    /* DELETE*/
    // returns pointer to sibling - CHANGED!
    Node *get_sibling(Node *X) {
        if (X->pParent == nullptr) return nullptr;
        if (X == X->pParent->pChild[0]) return X->pParent->pChild[1];
        return X->pParent->pChild[0];
    }

    Node *successor(Node *X) {
        Node *temp = X;
        while (temp->pChild[0] != nullptr)
            temp = temp->pChild[0];
        return temp;
    }
    // find node that replaces a deleted node in BST
    Node *BSTreplace(Node *x) {
        // when node have 2 children
        //Node **succesor = &(x->pChild[1]); // included
        //find_min(succesor);                 // included
        //x->data = (*succesor)->data;       // included
        if (x->pChild[0] == nullptr) return x->pChild[1];
        else if (x->pChild[1] == nullptr) return x->pChild[0];
        else return successor(x->pChild[1]);
    }
};
/* END OF HEADER */

/* ROTATION FUNCTIONS*/
void RBTree::left_rotate(Node *&pRoot, Node *&X) {
    Node *Y = X->pChild[1];

    //nova relacao de y.left (beta no livro)
    X->pChild[1] = Y->pChild[0];
    if (Y->pChild[0] != nullptr) // mudei para = livro
        X->pChild[1]->pParent = X; // livro parece errado neste caso, pois Y->left vai ser o X. Ver penultima linha desta funcao

    // nova relacao de X.pParent
    Y->pParent = X->pParent;
    if (X->pParent == nullptr) // se X era root, Y será novo root
        pRoot = Y;
    else if (X == X->pParent->pChild[0])
        X->pParent->pChild[0] = Y;
    else
        X->pParent->pChild[1] = Y;

    //nova relacao entre x e y
    Y->pChild[0] = X;
    X->pParent = Y;
}

void RBTree::right_rotate(Node *&pRoot, Node *&X) {
    Node *Y = X->pChild[0];

    //nova relacao de y.right (beta no livro)
    X->pChild[0] = Y->pChild[1];
    if (Y->pChild[1] != nullptr)
        X->pChild[0]->pParent = X;

    // nova relacao de X.pParent
    Y->pParent = X->pParent;
    if (X->pParent == nullptr)
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
Node* RBTree::RB_insert(Node* subTree, Node *X) {
    /* X será a nova raiz, se tree estiver vazia */
    if (subTree == nullptr) return X;

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
void RBTree::RB_insert_fixup(Node *&subTree, Node *&Z) {
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
            if (Y != nullptr && Y->color == RED) {
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
            if ((Y != nullptr) && (Y->color == RED)) {
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

void RBTree::RB_insert(int &data) {
    cout << data << " "<< " "<< &data << endl;
    Node *Z = new Node(data);
    cout << "Z: " << Z << " " << " " << Z << endl;

    // Do a normal BST insert
    pRoot = RB_insert(pRoot, Z);

    // fix Red Black Tree violations
    //Z->color = RED;
    RB_insert_fixup(pRoot, Z);
}

/* DELETION FUNCTIONS*/
// deletes the given node
void RBTree::RB_delete(Node *Z) {
    Node *pSibling;
    Node *pParent = Z->pParent;
    Node *X = BSTreplace(Z); // X is successor of Z, as in the book

    // True when X and Z are both black
    bool doubly_black = ((Z->color == BLACK) && (X == nullptr || X->color == BLACK));

    if (X == nullptr) {
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
                pSibling = get_sibling(Z);
                if (Z->pSibling != nullptr)
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

    if (Z->pChild[0] == nullptr || Z->pChild[1] == nullptr) {
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

    // Z has 2 children, swap values with successor and recurse
    swap(X->data, Z->data);
    RB_delete(X);
}

void RBTree::RB_delete_fixup(Node *X) {
    if (X == pRoot) return;

    Node *pParent = X->pParent;
    Node *pSibling = get_sibling(X);

    if (pSibling == nullptr) {
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
            if (pSibling->pChild[0] != nullptr && pSibling->pChild[0]->color == RED) {
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

void RBTree::RB_remove(int X) {
    if (pRoot == nullptr) return;
    Node *v = search(X);
    Node *u;
    if (v->data != X) return;
    RB_delete(v);
}

//template <typename... Ts>//void RBTree::process(int t, Ts... ts) {//    RB_insert(t);//    this->process(ts...);//}

RBTree::RBTree(initializer_list<int> values):pRoot(nullptr) {
    for (auto val: values) {
        this->RB_insert(val);
    }  }

int main() {
    RBTree rbt({41, 38, 31, 12, 19, 8});

    cout << "\n\narvore gerada\n";
    rbt.print();

    rbt.RB_remove(19);
    cout << "\n\narvore apos delecao\n";
    rbt.print();
    return 0;
}
