#include <bits/stdc++.h> 
#include <iostream>
#include <iomanip>
#include <initializer_list>
using namespace std; 

enum Color {RED, BLACK}; 

struct Node {
    int data; 
    bool color; 
    Node *pChild[2],  *pParent;

    Node(int data) {
    this->data = data; 
    pChild[0] = pChild[1] = pParent = NULL; 
    } 
}; 

//template<typename T>
class RBTree { 
protected: 
    Node *pRoot; 
    void left_rotate(Node *&, Node *&); 
    void right_rotate(Node *&, Node *&); 
    void RB_insert_fixup(Node *&, Node *&); 
public: 
    RBTree(initializer_list<int> values);
    
    RBTree() { pRoot = NULL; } 
    void insert(const int &n); 
    
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
    void remove(int x) {
        Node **p;
        if(find(x, p))
            remove(*p);
    }

    void print() {
        print(pRoot);
        cout << endl;
    }
   
    void inorder(); 
    void levelOrder(); 
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

// Constructor with Initialization List
RBTree::RBTree(initializer_list<int> values):pRoot(NULL) {
    for (auto val: values) {
        this->push_back(val);
    }
}

/* A utility function to insert a new node with given key in BST */
//falta o caso quando X já existe
Node* BSTInsert(Node* T, Node *X) { 
    /* X será a nova raiz, se tree estiver vazia */
    if (T == NULL) return X; 
    
    /* desce a árvore até encontrar um ponto para encaixar */
    else {
        if (X->data < T->data) 	{ 
            T->pChild[0] = BSTInsert(T->pChild[0], X); 
            T->pChild[0]->pParent = T; 
        } 
        else if (X->data > T->data) { 
            T->pChild[1] = BSTInsert(T->pChild[1], X); 
            T->pChild[1]->pParent = T; 
        } 
    }
    /* return the (unchanged) node pointer */
    return T; 
} 

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

// This function fixes violations caused by BST insertion 
void RBTree::RB_insert_fixup(Node *&pRoot, Node *&Z) { 
    Node *Z_pParent = NULL; 
    Node *Z_pGrpParent = NULL; 

    while ((Z != pRoot) && (Z->color != BLACK) && (Z->pParent->color == RED)) {
        Z_pParent = Z->pParent; 
        Z_pGrpParent = Z->pParent->pParent;
        
        /* Z.pParent is at left */
        if (Z_pParent == Z_pGrpParent->pChild[0]) { 
            Node *Y = Z_pGrpParent->pChild[1]; 

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if (Y != NULL && Y->color == RED) { 
                Z_pParent->color = BLACK; 
                Y->color = BLACK; 
                Z_pGrpParent->color = RED; 
                Z = Z_pGrpParent; 
            } 
            else{ 
                /* Z is at right: left-rotation of Z.p, then right-rotation of Z.p.p */
                if (Z == Z_pParent->pChild[1]) { 
                    left_rotate(pRoot, Z_pParent); 
                    Z = Z_pParent; // this is necessary for the other rotation to work correctly. Otherwise Z.p.p would be actually the grand_grand_pParent.
                    Z_pParent = Z->pParent; // redefine Z_p=Z.p, after rotation.
                } 
                /* Z is at left: only right-rotation of Z.p.p */
                right_rotate(pRoot, Z_pGrpParent); // apos rotacao, vai colocar pParent acima de grandpParent
                swap(Z_pParent->color, Z_pGrpParent->color); 
                Z = Z_pParent; 
            } 
        } 

        /* Z.p is at right */
        else{ 
            Node *Y = Z_pGrpParent->pChild[0]; 

            /* pParent and uncle are red. Recoloring both and Z.p.p */
            if ((Y != NULL) && (Y->color == RED)) { 
                Z_pParent->color = BLACK; 
                Y->color = BLACK; 
                Z_pGrpParent->color = RED; 
                Z = Z_pGrpParent; 
            } 
            else{ 
                /* Z is at left: right-rotation of Z.p, then left-rotation of Z.p.p */
                if (Z == Z_pParent->pChild[0]) { 
                    right_rotate(pRoot, Z_pParent); 
                    Z = Z_pParent; // this is necessary for the other rotation to work correctly. Otherwise Z.p.p would be actually the grand_grand_pParent.
                    Z_pParent = Z->pParent; // redefine Z_p=Z.p, after rotation.
                } 
                /* Z is at right: only left-rotation of Z.p.p */
                left_rotate(pRoot, Z_pGrpParent); // apos rotacao, vai colocar pParent acima de grandpParent
                swap(Z_pParent->color, Z_pGrpParent->color); 
                Z = Z_pParent; 
            } 
        } 
    } 

    pRoot->color = BLACK; // needs to assure that root is black
} 

// Function to insert a new node with given data 
void RBTree::insert(const int &data) { 
    Node *Z = new Node(data); 

    // Do a normal BST insert 
    pRoot = BSTInsert(pRoot, Z); 

    // fix Red Black Tree violations 
    RB_insert_fixup(pRoot, Z); 
} 

// A recursive function to do level order traversal 
void inorderHelper(Node *pRoot) { 
    if (pRoot == NULL) 
        return; 
    inorderHelper(pRoot->pChild[0]); 
    cout << pRoot->data << " "; 
    inorderHelper(pRoot->pChild[1]); 
} 

// Utility function to do level order traversal 
void levelOrderHelper(Node *pRoot) { 
    if (pRoot == NULL) 
        return; 
    std::queue<Node *> q; 
    q.push(pRoot); 
    while (!q.empty()) 	{ 
        Node *temp = q.front(); 
        cout << temp->data << " "; 
        q.pop(); 
        if (temp->pChild[0] != NULL) 
            q.push(temp->pChild[0]); 
        if (temp->pChild[1] != NULL) 
            q.push(temp->pChild[1]); 
    }
} 

// Function to do inorder and level order traversals 
void RBTree::inorder() { inorderHelper(pRoot);} 
void RBTree::levelOrder() { levelOrderHelper(pRoot); } 

// Driver Code 
int main() { 
    RBTree tree; 
    
    //RBTree tree (41 , 38 , 31 , 12 , 19 , 8);
    tree.insert(7); 
    tree.insert(6); 
    tree.insert(5); 
    tree.insert(4); 
    tree.insert(3); 
    tree.insert(2); 
    tree.insert(1); 

    cout << "Inoder Traversal of Created Tree\n"; 
    tree.inorder(); 

    cout << "\n\nLevel Order Traversal of Created Tree\n"; 
    tree.levelOrder(); 
    
    cout << "\n\nprint Created Tree\n"; 
    tree.print(); 

    return 0; 
} 
