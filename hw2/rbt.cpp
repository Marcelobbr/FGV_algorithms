#include <bits/stdc++.h> 
#include <iostream>
#include <iomanip>
#include <initializer_list>
using namespace std; 

//clear if unused: *pNext, initialization funcs
// swap -> verbose version

enum Color {RED, BLACK}; 

struct Node {
    int data; 
    bool color; 
    Node *pChild[2], *pParent, *pNext; // pNext NOT WORKING

    Node(int data) {
    this->data = data; 
    pChild[0] = pChild[1] = pParent = NULL; 
    } 
}; 

class RBTree { 
protected: 
    Node *pRoot; 
    void left_rotate(Node *&, Node *&); 
    void right_rotate(Node *&, Node *&); 
    void RB_insert_fixup(Node *&, Node *&); 
public: 
    //funcs to work with initialization list of ints, NOT WORKING
    RBTree(initializer_list<int> values);
    void push_back(int x);
    template<typename ...Ts>
    RBTree(Ts... ts);
    //funcs end
    
    RBTree() { pRoot = NULL; } 
    void insert(const int &n); 
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
   

private:     
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
}; 
/* END OF HEADER */

// constructors with push_back and initialization list, NOT WORKING
void RBTree::push_back(int x) {
    Node **pNode;
    if ((find(x, pNode))) return;
    Node *newNode = new Node(x);
    newNode->pNext = *pNode;
    *pNode = newNode;
}
RBTree::RBTree(initializer_list<int> values):pRoot(NULL) {
    for (auto val: values) {
        this->push_back(val);
    }
}
// test constructors end

/* ROTATION FUNCTIONS*/
void RBTree::left_rotate(Node *&pRoot, Node *&X) { 
    Node *Y = X->pChild[1]; 
    
    //nova relacao de y.left (beta no livro)
    X->pChild[1] = Y->pChild[0]; 
    if (Y->pChild[0] != NULL) // mudei para = livro
        X->pChild[1]->pParent = X; // livro parece errado neste caso, pois Y->left vai ser o X. Ver penultima linha desta funcao
    
    // nova relacao de X.pParent
    Y->pParent = X->pParent; 
    if (X->pParent == NULL) // se X era root, Y será novo root
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
    if (Y->pChild[1] != NULL) 
        X->pChild[0]->pParent = X; 
        
    // nova relacao de X.pParent
    Y->pParent = X->pParent; 
    if (X->pParent == NULL) 
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
/* A utility function to insert a new node with given key in BST */
//falta o caso quando X já existe
Node* BST_insert(Node* T, Node *X) { 
    /* X será a nova raiz, se tree estiver vazia */
    if (T == NULL) return X; 
    
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
    Node *Z_pParent = NULL; 
    Node *Z_pGrParent = NULL; 

    while ((Z != pRoot) && (Z->color != BLACK) && (Z->pParent->color == RED)) {
        Z_pParent = Z->pParent; 
        Z_pGrParent = Z->pParent->pParent;
        
        /* Z.pParent is at left */
        if (Z_pParent == Z_pGrParent->pChild[0]) { 
            Node *Y = Z_pGrParent->pChild[1]; 

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if (Y != NULL && Y->color == RED) { 
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
            if ((Y != NULL) && (Y->color == RED)) { 
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
void RBTree::insert(const int &data) { 
    cout << data << " "<< " "<< &data << endl;
    Node *Z = new Node(data); 
    cout << "Z: " << Z << " " << " " << Z << endl;

    // Do a normal BST insert 
    pRoot = BST_insert(pRoot, Z); 

    // fix Red Black Tree violations 
    Z->color = RED;
    RB_insert_fixup(pRoot, Z); 
} 


int main() { 
    RBTree tree; 
    
    //RBTree tree(41 , 38 , 31 , 12 , 19 , 8);
    tree.insert(7); 
    tree.insert(6); 
    tree.insert(5); 
    cout << "\n\nprint Partial Tree\n"; 
    tree.print(); 
    tree.insert(4); 
    tree.insert(3); 
    tree.insert(2); 
    tree.insert(1); 
    
    cout << "\n\nprint Created Tree\n"; 
    tree.print(); 

    return 0; 
} 
