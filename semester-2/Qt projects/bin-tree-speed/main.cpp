#include <QCoreApplication>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QDebug>
#include <vector>
#include <queue>
#include <QVector>
#include <iostream>

using namespace std;

class BinarySearchTree {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;

        Node(int value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, int value)
    {
        if (node == nullptr)
        {
            return new Node(value);
        }

        if (value < node->data)
        {
            node->left = insert(node->left, value);
        }
        else if (value > node->data)
        {
            node->right = insert(node->right, value);
        }

        return node;
    }

    bool search(Node* node, int value) const
    {
        if (node == nullptr)
        {
            return false;
        }

        if (value == node->data)
        {
            return true;
        }
        else if (value < node->data)
        {
            return search(node->left, value);
        }
        else {
            return search(node->right, value);
        }
    }

    Node* findMin(Node* node) const
    {
        while (node && node->left)
        {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, int value)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (value < node->data)
        {
            node->left = remove(node->left, value);
        }
        else if (value > node->data)
        {
            node->right = remove(node->right, value);
        }
        else
        {
            if (node->left == nullptr)
            {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* minNode = findMin(node->right);
            node->data = minNode->data;
            node->right = remove(node->right, minNode->data);
        }

        return node;
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node* copy(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        Node* newNode = new Node(node->data);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

public:

    void printEdges() const {
        Node* leftNode = root;
        while (leftNode && leftNode->left) {
            leftNode = leftNode->left;
        }
        Node* rightNode = root;
        while (rightNode && rightNode->right) {
            rightNode = rightNode->right;
        }
        std::cout << "┌─ Left (min): " << (leftNode ? leftNode->data : -1)
                  << "  │  Right (max): " << (rightNode ? rightNode->data : -1) << " ─┐\n";
    }

    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const std::vector<int>& values) : root(nullptr) {
        for (int value : values) {
            insert(value);
        }
    }

    BinarySearchTree(const BinarySearchTree& other) {
        root = copy(other.root);
    }

    ~BinarySearchTree() {
        clear(root);
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            clear(root);
            root = copy(other.root);
        }
        return *this;
    }

    void insert(int value) {
        root = insert(root, value);
    }

    bool search(int value) const {
        return search(root, value);
    }

    void remove(int value) {
        root = remove(root, value);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }
};

string formatCount(int count) {
    if (count >= 1000000) {
        return to_string(count / 1000000) + "M";
    } else if (count >= 1000) {
        return to_string(count / 1000) + "K";
    }
    return to_string(count);
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    const int ELEMENTS_COUNT = 10000000;
    const int SEARCH_COUNT = 1000000;

    vector<int> arr(ELEMENTS_COUNT);
    QElapsedTimer timer;

    timer.start();
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {

        arr[i] = QRandomGenerator::global()->bounded(1, 1000001);
    }

    double elapsed_time = timer.nsecsElapsed() / 1000000.0;


    qDebug().nospace() << "Time of execution (insert vector, " << QString::fromStdString(formatCount(ELEMENTS_COUNT)) << " elems) : " << elapsed_time << " milliseconds";


    timer.restart();
    BinarySearchTree bst(arr);
    elapsed_time = timer.nsecsElapsed() / 1000000.0;

    qDebug().nospace() << "\nTime of execution (creating bst, " << QString::fromStdString(formatCount(ELEMENTS_COUNT)) << " elems)  : " << elapsed_time << " milliseconds";


    timer.restart();
    for (int i = 0; i < SEARCH_COUNT; ++i) {
        bst.search(i * 5);
    }
    elapsed_time = timer.nsecsElapsed() / 1000000.0;

    qDebug().nospace() << "Time of execution (searching bst, " << QString::fromStdString(formatCount(SEARCH_COUNT)) << " ops) : " << elapsed_time << " milliseconds";

    bst.printEdges();

    timer.restart();
    queue<int, QVector<int>> que;
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        que.push(QRandomGenerator::global()->bounded(1, 1000001));
    }
    elapsed_time = timer.nsecsElapsed() / 1000000.0;

    qDebug().nospace() << "\nTime of execution (creating queue on vector, " << QString::fromStdString(formatCount(ELEMENTS_COUNT)) << " elems)  : " << elapsed_time << " milliseconds";


    timer.restart();
    for (int i = 0; i < SEARCH_COUNT; ++i) {
        que.pop();
    }
    elapsed_time = timer.nsecsElapsed() / 1000000.0;

    qDebug().nospace() << "Time of execution (pop queue on vector, " << QString::fromStdString(formatCount(SEARCH_COUNT)) << " ops) : " << elapsed_time << " milliseconds";

    return 0;
}
