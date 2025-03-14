#include "bst.h"

Node *BST::add(Node *root, Data x)
{
    if(root==nullptr){
        Node *t = new Node;
        t->data = x;
        t->left = t->right= nullptr;
        return t;
    }
    if(x.key < root->data.key){
        root->left = add(root->left,x);
    }
    if(x.key > root->data.key){
        root->right = add(root->right,x);
    }
    //return root;
   root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    return balance(root);
}

Node *BST::remove(Node *root, int key)
{
    if (root == nullptr) {
        return root;
    }
    if (key < root->data.key) {
        root->left = remove(root->left, key);
    } else if (key > root->data.key) {
        root->right = remove(root->right, key);
    } else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        Node* temp = findLeft(root->right);
        root->data = temp->data;
        root->right = remove(root->right, temp->data.key);
    }
    //return root;
   root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    return balance(root);
}

Node *BST::removeBrunch(Node *root, int key)
{
    if (root == nullptr) {
        return root;
    }
    if (key < root->data.key) {
        root->left = remove(root->left, key);
    } else if (key > root->data.key) {
        root->right = remove(root->right, key);
    } else {
        clear(root);
        return nullptr;
    }
    return root;
}

Node *BST::findLeft(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

Node *BST::search(Node *root, int key)
{
    if (root == nullptr || root->data.key == key) {
        return root;
    }
    if (key < root->data.key) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

void BST::printPreorder(Node *root, QTextEdit& textEdit)
{
    if (root == nullptr) {
        return;
    }
    textEdit.append(QString::number(root->data.key) + " " + root->data.string + " ");
    printPreorder(root->left, textEdit);
    printPreorder(root->right, textEdit);
}

void BST::printPostorder(Node *root, QTextEdit& textEdit)
{
    if (root == nullptr) {
        return;
    }
    printPostorder(root->left, textEdit);
    printPostorder(root->right, textEdit);
    textEdit.append(QString::number(root->data.key) + " " + root->data.string + " ");
}

void BST::printInorder(Node *root, QTextEdit& textEdit)
{
    if(root==nullptr){
        return;
    }
    printInorder(root->left, textEdit);
    textEdit.append(QString::number(root->data.key) + " " + root->data.string + " ");
    printInorder(root->right, textEdit);
}

void BST::clear(Node* root) {
    if (root != nullptr) {
        clear(root->left);
        clear(root->right);
        delete root;
        root = nullptr;
    }
}

int BST::getHeight(Node* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

int BST::getBalance(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* BST::rotateLeft(Node* node) {
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

Node* BST::rotateRight(Node* node) {
    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

Node* BST::balance(Node* node) {
    if (node == nullptr) return nullptr;

    node->left = balance(node->left);
    node->right = balance(node->right);

    int balanceFactor = getBalance(node);

    if (balanceFactor > 1) {
        if (getBalance(node->left) >= 0) {
            return rotateRight(node);
        } else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (balanceFactor < -1) {
        if (getBalance(node->right) <= 0) {
            return rotateLeft(node);
        } else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

void BST::insertSubtree(Node*& root, Node* subtree) {
    if (subtree == nullptr) {
        return;
    }
    insertSubtree(root, subtree->left);
    if (search(root, subtree->data.key) == nullptr) {
        root = add(root, subtree->data);
    }
    insertSubtree(root, subtree->right);
}

void BST:: removeSubtree(Node*& root, Node* subtree) {
    if (subtree == nullptr) {
        return;
    }
    removeSubtree(root, subtree->left); // Обрабатываем левое поддерево
    removeSubtree(root, subtree->right);
    if (search(root, subtree->data.key) != nullptr) {
        root = remove(root, subtree->data.key); // Удаляем узел из основного дерева
    }

}


Node* insertAt(Node* root, Data data, std::vector<int> path) {
    Node* current = root;
    for (int key : path) {
        if (key < current->data.key) {
            if (current->left == nullptr) {
                current->left = new Node{data, nullptr, nullptr};
                return root;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new Node{data, nullptr, nullptr};
                return root;
            }
            current = current->right;
        }
    }
    return root;
}

Node*  BST:: findAncestor(Node* root, int key1, int key2) {
    if (root == nullptr || root->data.key == key1 || root->data.key == key2) {
        return root;
    }
    Node* left = findAncestor(root->left, key1, key2);
    Node* right = findAncestor(root->right, key1, key2);
    if (left != nullptr && right != nullptr) {
        return root;
    }
    return (left != nullptr) ? left : right;
}
