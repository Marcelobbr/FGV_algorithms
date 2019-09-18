#include <iostream>
#include <iomanip>

using namespace std;

struct Node {
    int data;
    Node *pChild[2];

    Node(int x):data(x) {
        pChild[0] = pChild[1] = nullptr;
    }
};

class BST {
protected:
    Node *pRoot;

public:
    BST():pRoot(nullptr) {}

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

int main() {
    BST bst;
    bst.insert(6);
    bst.insert(4);
    bst.insert(7);
    bst.insert(2);
    bst.insert(5);
    bst.insert(1);
    bst.insert(3);
    bst.print();

    cout << "-----------------\n";
    bst.remove(4);
    bst.print();

    bst.remove(3);
    bst.print();

    bst.remove(6);
    bst.print();

    return 0;
}