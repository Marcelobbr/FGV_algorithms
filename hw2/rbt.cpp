#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include <vector>
#include <string>
using namespace std;

//clear if unused: *pNext, initialization funcs

enum Color {RED, BLACK};

struct Node {
    int data;
    bool color;
    Node *pChild[2], *pParent, *pSibling, *pNext; // pNext NOT WORKING
    Node(int x):data(x) {
        pChild[0] = pChild[1] = pParent = pSibling = pNext = nullptr;
    }
};

class RBTree {
protected:
    Node *pRoot;
public:
    //funcs to work with initialization list of ints, NOT WORKING
    //RBTree(initializer_list<int> values);
    //void push_back(int x);
    //template<typename ...Ts>
    //RBTree(Ts... ts);
    //funcs end

    RBTree() { pRoot = nullptr; }
    void remove_and_recolor(int);
    void insert_and_recolor(const int &n);
    //void insert(const int &x) { //void RBTree::insert(const int &data) {
     //   Node **p;        if (!find(x, p)) {*p = new Node(x);        }    }

    bool find(int x) {
        Node **p;
        return find(x, p);
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
    Node *search(int x) {
        Node *temp = pRoot;
        while (temp != nullptr) {
            if (x < temp->data) {
                if (temp->pChild[0] == nullptr) break;
                else temp = temp->pChild[0];
            }
            else if (x == temp->data) break;
            else {
                if (temp->pChild[1] == nullptr) break;
                else temp = temp->pChild[1];
            }
        }
        return temp;
    }
private:
    void left_rotate(Node *&, Node *&);
    void right_rotate(Node *&, Node *&);
    void RB_insert_fixup(Node *&, Node *&);
    void RB_delete(Node *);
    void RB_delete_fixup(Node *);
    bool find(int x, Node **&p) {
        p = &pRoot;
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

    Node *tree_successor(Node *X) {
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

        if (x->pChild[0] != nullptr && x->pChild[1] != nullptr)
        return tree_successor(x->pChild[1]);

        // when leaf
        if (x->pChild[0] == nullptr && x->pChild[1] == nullptr)
        return nullptr;

        // when single child
        if (x->pChild[0] != nullptr) return x->pChild[0];
        else return x->pChild[1];
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
Node* BST_insert(Node* T, Node *X) {
    /* X será a nova raiz, se tree estiver vazia */
    if (T == nullptr) return X;

    /* desce a árvore até encontrar um ponto para encaixar */
    else {
        if (X->data < T->data) 	{
            T->pChild[0] = BST_insert(T->pChild[0], X);
            T->pChild[0]->pParent = T;
        }
        else if (X->data > T->data) {
            T->pChild[1] = BST_insert(T->pChild[1], X);
            T->pChild[1]->pParent = T;
        }
    }
    /* return the (unchanged) node pointer */
    return T;
}

// This function fixes violations caused by BST insertion
void RBTree::RB_insert_fixup(Node *&pRoot, Node *&Z) {
    Node *Z_pParent = nullptr;
    Node *Z_pGrParent = nullptr;
    Z->color = RED;

    while ((Z != pRoot) && (Z->color != BLACK) && (Z->pParent->color == RED)) {
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
                    left_rotate(pRoot, Z_pParent);
                    Z = Z_pParent; // this is necessary for the other rotation to work correctly. Otherwise Z.p.p would be actually the grand_grand_pParent.
                    Z_pParent = Z->pParent; // redefine Z_p=Z.p, after rotation.
                }
                /* Z is at left: only right-rotation of Z.p.p */
                right_rotate(pRoot, Z_pGrParent); // apos rotacao, vai colocar pParent acima de grandpParent
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
                    right_rotate(pRoot, Z_pParent);
                    Z = Z_pParent; // this is necessary for the other rotation to work correctly. Otherwise Z.p.p would be actually the grand_grand_pParent.
                    Z_pParent = Z->pParent; // redefine Z_p=Z.p, after rotation.
                }
                /* Z is at right: only left-rotation of Z.p.p */
                left_rotate(pRoot, Z_pGrParent); // apos rotacao, vai colocar pParent acima de grandpParent
                swap(Z_pParent->color, Z_pGrParent->color);
                Z = Z_pParent;
            }
        }
    }

    pRoot->color = BLACK; // needs to assure that root is black
}

// Function to insert a new node with given data
void RBTree::insert_and_recolor(const int &data) {
    cout << data << " "<< " "<< &data << endl;
    Node *Z = new Node(data);
    cout << "Z: " << Z << " " << " " << Z << endl;

    // Do a normal BST insert
    pRoot = BST_insert(pRoot, Z);

    // fix Red Black Tree violations
    //Z->color = RED;
    RB_insert_fixup(pRoot, Z);
}

/* DELETION FUNCTIONS*/
// deletes the given node
void RBTree::RB_delete(Node *v) {
    Node *pSibling;
    Node *pParent = v->pParent;
    Node *u = BSTreplace(v);

    // True when u and v are both black
    bool doubly_black = ((v->color == BLACK) && (u == nullptr || u->color == BLACK));

    if (u == nullptr) {
        // u is NULL therefore v is leaf
        if (v == pRoot) {
            pRoot = nullptr;
        }
        else {
            if (doubly_black) {
                // u and v both black
                // v is leaf, fix double black at v
                RB_delete_fixup(v);
            }
            else {
                // u or v is red
                pSibling = get_sibling(v);
                if (v->pSibling != nullptr)
                    // sibling is not null, make it red"
                    v->pSibling->color = RED;
            }

            // delete v from the tree
            if (v == v->pParent->pChild[0]) {
                pParent->pChild[0] = nullptr;
            }
            else {
                pParent->pChild[1] = nullptr;
            }
        }
        delete v; return;
    }

    if (v->pChild[0] == nullptr || v->pChild[1] == nullptr) {
        // v has 1 child
        if (v == pRoot) {
            // v is root, assign the value of u to v, and delete u
            v->data = u->data;
            v->pChild[0] = v->pChild[1] = nullptr;
            delete u;
        }
        else {
            // Detach v from tree and move u up
            if (v == v->pParent->pChild[0]) {
                pParent->pChild[0] = u;
            }
            else {
                pParent->pChild[1] = u;
            }
            delete v;
            u->pParent = pParent;
            if (doubly_black) {
                // u and v both black, fix double black at u
                RB_delete_fixup(u);
            }
            else {
                // u or v red, color u black
                u->color = BLACK;
            }
        }
        return;
    }

    // v has 2 children, swap values with successor and recurse
    swap(u->data, v->data);
    RB_delete(u);
}

void RBTree::RB_delete_fixup(Node *x) {
    if (x == pRoot) return;

    Node *pParent = x->pParent;
    Node *pSibling = get_sibling(x);

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
            RB_delete_fixup(x);
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

void RBTree::remove_and_recolor(int X) {
    if (pRoot == nullptr) return;
    Node *v = search(X);
    //Node *u;
    if (v->data != X) return;
    RB_delete(v);
}

// constructors with push_back and initialization list, NOT WORKING
//void RBTree::push_back(int x) {
//    Node **pNode;
//    if ((find(x, pNode))) return;
//    Node *newNode = new Node(x);
//    newNode->pNext = *pNode;
//    *pNode = newNode;
//}
//RBTree::RBTree(initializer_list<int> values):pRoot(nullptr) {
//    for (auto val: values) {
//        this->push_back(val);
//    }
//}
// test constructors end

int main() {
    //RBTree rbt(41 , 38 , 31 , 12 , 19 , 8);
    RBTree rbt; int sets[] = {41 , 38 , 31 , 12 , 19 , 8}; for (int h =0; h<=5; h++) {rbt.insert_and_recolor(sets[h]);}

    cout << "\n\narvore gerada\n";
    rbt.print();

    rbt.remove_and_recolor(19);
    //rbt.remove(19);
    cout << "\n\narvore apos delecao\n";
    rbt.print();
    return 0;
}
