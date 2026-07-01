#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

class BinarySearchTree {
private:
    struct Node {
        int data;
        Node *left;
        Node *right;

        Node(int value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node *root;

    Node *insert(Node *node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->data) {
            node->left = insert(node->left, value);
        } else if (value > node->data) {
            node->right = insert(node->right, value);
        }

        return node;
    }

    bool search(Node *node, int value) const {
        if (node == nullptr) {
            return false;
        }

        if (value == node->data) {
            return true;
        } else if (value < node->data) {
            return search(node->left, value);
        } else {
            return search(node->right, value);
        }
    }

    Node *findMin(Node *node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node *remove(Node *node, int value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value < node->data) {
            node->left = remove(node->left, value);
        } else if (value > node->data) {
            node->right = remove(node->right, value);
        } else {
            if (node->left == nullptr) {
                Node *temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node *temp = node->left;
                delete node;
                return temp;
            }

            Node *minNode = findMin(node->right);
            node->data = minNode->data;
            node->right = remove(node->right, minNode->data);
        }

        return node;
    }

    void clear(Node *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void inorder(Node *node, vector<int> &result) const {
        if (node) {
            inorder(node->left, result);
            result.push_back(node->data);
            inorder(node->right, result);
        }
    }

    void preorder(Node *node, vector<int> &result) const {
        if (node) {
            result.push_back(node->data);
            preorder(node->left, result);
            preorder(node->right, result);
        }
    }

    void postorder(Node *node, vector<int> &result) const {
        if (node) {
            postorder(node->left, result);
            postorder(node->right, result);
            result.push_back(node->data);
        }
    }

    void printTree(Node *node, int space, int indent) const {
        if (node == nullptr) {
            return;
        }

        space += indent;

        printTree(node->right, space, indent);

        cout << endl;
        for (int i = indent; i < space; i++) {
            cout << " ";
        }
        cout << node->data << endl;

        printTree(node->left, space, indent);
    }

    Node *copy(Node *node) {
        if (node == nullptr) {
            return nullptr;
        }

        Node *newNode = new Node(node->data);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const vector<int> &values) : root(nullptr) {
        for (int value : values) {
            insert(value);
        }
    }

    BinarySearchTree(const BinarySearchTree &other) {
        root = copy(other.root);
    }

    ~BinarySearchTree() {
        clear(root);
    }

    BinarySearchTree &operator=(const BinarySearchTree &other) {
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

    bool isEmpty() const {
        return root == nullptr;
    }

    vector<int> inorder() const {
        vector<int> result;
        inorder(root, result);
        return result;
    }

    vector<int> preorder() const {
        vector<int> result;
        preorder(root, result);
        return result;
    }

    vector<int> postorder() const {
        vector<int> result;
        postorder(root, result);
        return result;
    }

    vector<int> levelOrder() const {
        vector<int> result;
        if (root == nullptr) {
            return result;
        }

        queue<Node *> q;
        q.push(root);

        while (!q.empty()) {
            Node *current = q.front();
            q.pop();
            result.push_back(current->data);

            if (current->left) {
                q.push(current->left);
            }
            if (current->right) {
                q.push(current->right);
            }
        }

        return result;
    }

    int getMin() const {
        if (isEmpty()) {
            throw runtime_error("Tree is empty");
        }
        Node *minNode = findMin(root);
        return minNode->data;
    }

    int getMax() const {
        if (isEmpty()) {
            throw runtime_error("Tree is empty");
        }

        Node *maxNode = root;
        while (maxNode && maxNode->right) {
            maxNode = maxNode->right;
        }
        return maxNode->data;
    }

    int getSize() const {
        vector<int> nodes = inorder();
        return static_cast<int>(nodes.size());
    }

    void print() const {
        if (isEmpty()) {
            cout << "Tree is empty" << endl;
            return;
        }
        printTree(root, 0, 1);
        cout << endl;
    }

    void clear() {
        clear(root);
        root = nullptr;
    }
};

int main() {
    const int initialElementCount = 1000000;
    const int extraInsertCount = 10000;
    const int minValue = 1;
    const int maxValue = 1000000000;
    const uint32_t initialSeed = 123456789u;
    const uint32_t extraSeed = 987654321u;

    BinarySearchTree bst;
    uniform_int_distribution<int> distrib(minValue, maxValue);

    try {
        auto start = high_resolution_clock::now();
        mt19937 initialInsertGenerator(initialSeed);
        for (int i = 0; i < initialElementCount; ++i) {
            bst.insert(distrib(initialInsertGenerator));
        }
        auto end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(end - start).count();
        cout << "Time of execution (creating bst from 10000000 random elements) : "
             << elapsed << " milliseconds" << endl;

        start = high_resolution_clock::now();
        mt19937 extraInsertGenerator(extraSeed);
        for (int i = 0; i < extraInsertCount; ++i) {
            bst.insert(distrib(extraInsertGenerator));
        }
        end = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(end - start).count();
        cout << "Time of execution (inserting 10000 more elements) : "
             << elapsed << " milliseconds" << endl;

        start = high_resolution_clock::now();
        mt19937 initialRemoveGenerator(initialSeed);
        for (int i = 0; i < initialElementCount; ++i) {
            bst.remove(distrib(initialRemoveGenerator));
        }

        mt19937 extraRemoveGenerator(extraSeed);
        for (int i = 0; i < extraInsertCount; ++i) {
            bst.remove(distrib(extraRemoveGenerator));
        }
        end = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(end - start).count();
        cout << "Time of execution (removing all generated elements) : "
             << elapsed << " milliseconds" << endl;

        cout << "Tree is empty after removal: "
             << (bst.isEmpty() ? "true" : "false") << endl;
    } catch (const bad_alloc &) {
        cerr << "Not enough memory for 100000000 elements in this environment." << endl;
        return 1;
    } catch (const exception &ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}
