#include <iostream>

using namespace std;

// Структура для узла дерева
struct Node {
    int data;
    Node* left;
    Node* right;
};

// Функция для создания нового узла
Node* createNode(int value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Функция для вставки нового значения в дерево
Node* insert(Node* root, int value) {
    if (root == nullptr) {
        return createNode(value);
    }
    
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    
    return root;
}

// Функция прямого обхода (preorder) с выводом значений
void preorder(Node* root) {
    if (root != nullptr) {
        // 1. Посетить корень
        cout << root->data << " ";
        
        // 2. Обойти левое поддерево
        preorder(root->left);
        
        // 3. Обойти правое поддерево
        preorder(root->right);
    }
}

int main() {
    Node* root = nullptr;
    
    // Создаем дерево
    root = insert(root, 5);
    insert(root, 3);
    insert(root, 7);
    insert(root, 2);
    insert(root, 4);
    insert(root, 6);
    insert(root, 8);
    
    // Выводим дерево методом прямого обхода
    cout << "Preorder: ";
    preorder(root);
    cout << endl;
    
    return 0;
}