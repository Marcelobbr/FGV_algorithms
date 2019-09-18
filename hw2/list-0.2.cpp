#include <iostream>
#include <initializer_list>
#include <iomanip>      // std::setw

using namespace std;

template<typename T>
struct Node{
    T data;
    Node *pNext;

    Node(T x):data(x), pNext(NULL){}
};

struct NotFoundException : public exception {
    const char *what () const throw () {
        return "Element not found";
    }
};

template<typename T>
class LinkedList{
protected:
    Node<T> *pRoot;
    int length;


public:
    // Constructor with Initialization List
    LinkedList(initializer_list<T> values);

    // Constructor with Variadic Templates
    template<typename ...Ts>
    LinkedList(Ts... ts);

    // Destructor
    ~LinkedList();

    // finns and element in the lsit
    bool find(T x);

    // add and element at the ene if it does not exist
    void push_back(T x);

    // remove and element it is exits
    void remove(T x);

    // print the elements in the terminal
    void print();

    class Iterator {
    public:
        Node<T> *pNodo;
    public:
        Iterator(): pNodo(NULL) {}
        Iterator(Node<T> *p): pNodo(p) {}

        bool operator!=(Iterator it) { return this->pNodo != it.pNodo; }
        Iterator operator++(){ this->pNodo = pNodo->pNext; return *this; }
        T& operator*(){return pNodo->data;}
    };

    Iterator begin() { return this->pRoot; }
    Iterator end() { return NULL; }

private:
    // The base case
    void process() {} // end recursion

    // The recursive case
    template <typename... Ts>
    void process(T t, Ts... ts);

    // Private find function
    bool find(T x, Node<T> **&pNode);
};

// Constructor with Initialization List
template<typename T>
LinkedList<T>::LinkedList(initializer_list<T> values):pRoot(NULL), length(0) {
    for (auto val: values) {
        this->push_back(val);
    }
}

// Constructor with Variadic Templates
template<typename T>
template<typename ...Ts>
LinkedList<T>::LinkedList(Ts... ts): pRoot(NULL), length(0) {
    process(ts...);
}

// Destructor
template<typename T>
LinkedList<T>::~LinkedList() {
    Node<T> *temp;
    while(pRoot) {
        temp = pRoot;
        pRoot = pRoot->pNext;
        delete temp;
    }
}

template<typename T>
bool LinkedList<T>::find(T x) {
    Node<T> **pNode;
    return this->find(x, pNode);
}

template<typename T>
void LinkedList<T>::push_back(T x) {
    Node<T> **pNode;
    if ((find(x, pNode))) return;
    Node<T> *newNode = new Node<T>(x);
    newNode->pNext = *pNode;
    *pNode = newNode;
    length++;
}

template<typename T>
void LinkedList<T>::remove(T x) {
    Node<T> **pNode;
    if(!find(x, pNode)) throw NotFoundException();
    *pNode = (*pNode)->pNext;
}

template<typename T>
void LinkedList<T>::print() {
    Node<T> *obj = pRoot;
    while (obj) {
        cout << obj->data << " ";
        obj = obj->pNext;
    }
    cout << endl;
}

// The recursive case
template<typename T>
template <typename... Ts>
void LinkedList<T>::process(T t, Ts... ts) {
    this->push_back(t);
    this->process(ts...);
}

template<typename T>
bool LinkedList<T>::find(T x, Node<T> **&pNode) {
    pNode = &pRoot;
    while(*pNode) {
        if ((*pNode)->data==x) return true;
        pNode = &((*pNode)->pNext);
    }
    return false;
}


//// Constructor
//int main() {
//    // Option 1: Variadic Templates
//    LinkedList<int> list1(1, 2, 10, 2, 3);
//    list1.print();
//    // Option 2: Initialization List
//    LinkedList<int> list2({1, 2, 10, 2, 3});
//    list1.print();
//}


//// Templates
//int main() {
//    // create a linked list for three data types
//    LinkedList<int> ilist(1, 10, 2);
//    ilist.print();
//    LinkedList<float> flist(1.2, 1.4, 100000);
//    flist.print();
//    LinkedList<std::string> slist("one", "two", "three");
//    slist.print();
//}

//// Iterators
//int main() {
//    LinkedList<int> ilist(1, 2, 10, 2, 3);
//    LinkedList<int>::Iterator it;
//    for (it=ilist.begin(); it!=ilist.end(); ++it) {
//        std::cout << *it << " ";
//    }
//    cout << endl;
//}

int main() {
    LinkedList<int> ilist(1, 2, 10, 2, 3);
    // if we remove an item that doesn't exist it should throw an error
//    ilist.remove(20);

    // Correct way to handle exceptions
    try {
        ilist.remove(20);
    } catch (const NotFoundException& e) {
        cerr << e.what();
    }
}