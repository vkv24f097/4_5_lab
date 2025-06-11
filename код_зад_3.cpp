#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class BSTNode {
public:
    T data;
    BSTNode<T>* left;
    BSTNode<T>* right;
    
    BSTNode(T val) : data(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BST {
private:
    BSTNode<T>* root;
    int size;
    
    // Вспомогательная функция для рекурсивного поиска медианы
    BSTNode<T>* findMedian(BSTNode<T>* node, int& counter, int targetPos) {
        if (node == nullptr) {
            return nullptr;
        }
        
        // Ищем в левом поддереве
        BSTNode<T>* leftResult = findMedian(node->left, counter, targetPos);
        if (leftResult != nullptr) {
            return leftResult;
        }
        
        // Увеличиваем счетчик и проверяем текущий узел
        counter++;
        if (counter == targetPos) {
            return node;
        }
        
        // Ищем в правом поддереве
        return findMedian(node->right, counter, targetPos);
    }
    
public:
    BST() : root(nullptr), size(0) {}
    
    // Метод для получения медианы
    T median() {
        if (root == nullptr || size % 2 == 0) {
            cout << ("Дерево пустоe или имеет четное количество узлов") << endl;
        }
        
        int counter = 0;
        int targetPos = size / 2 + 1; // Позиция медианы
        BSTNode<T>* medianNode = findMedian(root, counter, targetPos);
        
        return medianNode->data;
    }
    
    // Метод для вставки (для тестирования)
    void insert(T val) {
        BSTNode<T>* newNode = new BSTNode<T>(val);
        if (root == nullptr) {
            root = newNode;
        } else {
            BSTNode<T>* current = root;
            BSTNode<T>* parent = nullptr;
            
            while (current != nullptr) {
                parent = current;
                if (val < current->data) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            
            if (val < parent->data) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        }
        size++;
    }
};

int main() {
    BST<int> bst;
    
    // Вставляем элементы в BST
    bst.insert(50);
    bst.insert(3);
    bst.insert(73);
    bst.insert(2);
    bst.insert(24);
    bst.insert(6);
    bst.insert(81);
    
    int med = bst.median();
    cout << "Медиана: " << med << endl;  
    
    return 0;
}