#include <iostream>

template <class T>
class BinarySearchTree {
public:
    struct Node {
        Node* left;
        Node* right;
        T value;
        
        Node(const T& val) : left(nullptr), right(nullptr), value(val) {}
    };

    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() {
        DestroyTree(root);
    }

    void Add(const T& value) {
        if (root == nullptr) {
            root = new Node(value);
        } else {
            AddNode(root, value);
        }
    }

    Node* Find(const T& value) {
        return FindNode(root, value);
    }

    void Delete(Node* node) {
        if (node == nullptr) return;
        root = DeleteNode(root, node->value);
    }

private:
    Node* root;

    void DestroyTree(Node* node) {
        if (node != nullptr) {
            DestroyTree(node->left);
            DestroyTree(node->right);
            delete node;
        }
    }

    void AddNode(Node* node, const T& value) {
        if (value < node->value) {
            if (node->left == nullptr) {
                node->left = new Node(value);
            } else {
                AddNode(node->left, value);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(value);
            } else {
                AddNode(node->right, value);
            }
        }
    }

    Node* FindNode(Node* node, const T& value) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if (value == node->value) {
            return node;
        } else if (value < node->value) {
            return FindNode(node->left, value);
        } else {
            return FindNode(node->right, value);
        }
    }

    Node* FindMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* DeleteNode(Node* node, const T& value) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if (value < node->value) {
            node->left = DeleteNode(node->left, value);
        } else if (value > node->value) {
            node->right = DeleteNode(node->right, value);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            } else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            } else {
                Node* temp = FindMin(node->right);
                node->value = temp->value;
                node->right = DeleteNode(node->right, temp->value);
            }
        }
        return node;
    }
};