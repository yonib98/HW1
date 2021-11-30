#ifndef HW1_MIVNEY_AVLTREE_H
#define HW1_MIVNEY_AVLTREE_H
#include <cmath>
#include "memory"
#include "string"
#include <iostream>
struct Trunk
{
    Trunk *prev;
    std::string str;

    Trunk(Trunk *prev, std::string str)
    {
        this->prev = prev;
        this->str = str;
    }
    void showTrunks(Trunk *p)
    {
        if (p == nullptr) {
            return;
        }

        showTrunks(p->prev);
        std::cout << p->str;
    };
};
template <class T>
class AVLTree {
    class Node {
        int key_primary;
        int key_secondary;
        T data;
        Node *parent;
        Node *left;
        Node *right;
        int height;
        friend class AVLTree<T>;
    public:
        Node(): parent(nullptr),left(nullptr),right(nullptr),height(0){};
        int getBf(){
            int left_height = left == nullptr ? -1 : left->height;
            int right_height = right == nullptr ? -1 : right->height;
            return left_height - right_height;
        }
        void updateHeight(){
            int lheight = left==nullptr? -1:left->height;
            int rheight = right ==nullptr? -1:right->height;
            height = std::max(lheight,rheight)+1;
        }
        bool isLeaf(){
            return right==nullptr && left == nullptr;
        }
        bool isOnlySingleChild(){
            return (right==nullptr && left!= nullptr) || (right!=nullptr && left==nullptr);
        }
        int getKey(){
            return key_primary;
        }
        bool operator==(const Node &to_compare) const;

        bool operator>(const Node &to_compare) const;
    };

    bool use_secondary_key;
    Node *root;
    Node *biggest;


    void updateBiggest();

    Node *innerFind(const Node &to_search);

    Node* findSequential(Node* p);

    void rightRotation(Node *current_root, Node *root_left_son);

    void leftRotation(Node *current_root, Node *root_right_son);

    Node* innerRemove(Node* to_find);

    void treeClear(Node* root);

    void printTree(Node* root, Trunk *trunks, bool isLeft);

public:
    AVLTree(bool use_secondary_key);

    const T& find(int key_primary, int key_secondary);

    void insert(int key_primary, int key_secondary, T data);

    void remove(int key_primary, int key_secondary);

    bool isEmpty() const;

    const T& getBiggest() const;

    void print();

    void deleteTree();

    ~AVLTree();
};
template <class T>
AVLTree<T>::AVLTree(bool use_secondary_key): use_secondary_key(use_secondary_key), root(nullptr),biggest(nullptr){}

template<class T>
AVLTree<T>::~AVLTree() {
    deleteTree();
}
template <class T>
typename AVLTree<T>::Node* AVLTree<T>::innerFind(const Node& to_search){
    Node* closest_parent = nullptr;
    Node* temp=root;
    while(temp!=nullptr){
        if(to_search==*temp) {
            return temp;
        }
        else if (to_search>*temp){
            closest_parent=temp;
            temp=temp->right;
        }
        else{
            closest_parent=temp;
            temp=temp->left;

        }
    }
    return closest_parent;
}

template <class T>
const T& AVLTree<T>::find(int key_primary,int key_secondary){
    Node to_search;
    to_search.key_primary = key_primary;
    to_search.key_secondary=key_secondary;
    Node* result = innerFind(to_search);
    if(*result==to_search){
        return result->data;
    }
    throw std::exception();

}

template <class T>
void AVLTree<T>::insert(int key_primary,int key_secondary,T data) {
    Node *to_insert = new Node();
    to_insert->key_primary = key_primary;
    if(use_secondary_key) {
        to_insert->key_secondary = key_secondary;
    }
    else{
        to_insert->key_secondary = 0;
    }
    to_insert->data = data;
    Node *result = innerFind(*to_insert);
    if(result==nullptr){
        root=to_insert;
        updateBiggest();
        return;
    }
    if (*result == *to_insert) {
        //AlreadyExists
        delete to_insert;
        throw std::exception();
    }
    if (*to_insert > *result) {
        result->right = (to_insert);
        to_insert->parent = result;
    } else {
        result->left = (to_insert);
        to_insert->parent = result;
    }
    Node *temp = to_insert;
    bool rotation = false;
    while (temp != root && !rotation) {
        Node* parent = temp->parent;
        if(parent->height >= temp->height+1){
            break;
        }else{
            parent->height = temp->height+1;
        }
        int current_bf = parent->getBf();
        if (current_bf == 2) {
            rotation = true;
            Node *left_son = parent->left;
            int left_bf = left_son->getBf();
            if (left_bf >= 0) {
                if(parent->parent== nullptr)
                    root=left_son;
                else{
                    parent->parent->left = left_son;
                }
                rightRotation(parent, left_son);
                //LL -> right
            } else {

                Node *left_right_son = left_son->right;
                parent->left = (left_right_son);
                leftRotation(left_son, left_right_son);

                if(parent->parent== nullptr){
                    root=left_right_son;
                }
                else{
                    parent->parent->left=left_right_son;
                }
                rightRotation(parent, left_right_son);
                //LR
            }
        } else if (current_bf == -2) {
            rotation = true;
            Node *right_son = parent->right;
            int right_bf = right_son->getBf();
            if (right_bf <= 0) {
                //RR
                if(parent->parent== nullptr)
                    root=right_son;
                else{
                    parent->parent->right = right_son;
                }
                leftRotation(parent, right_son);
            } else {

                Node *right_left_son = right_son->left;
                parent->right = (right_left_son);
                rightRotation(right_son, right_left_son);

                if (parent->parent == nullptr) {
                    root = right_left_son;
                } else {
                    parent->parent->right = right_left_son;
                }
                leftRotation(parent, right_left_son);
                //RL
            }
        }
        temp=parent;
    }
    updateBiggest();
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::findSequential( typename AVLTree<T>::Node* p){
    p=p->right;
    while(p->left!= nullptr){
        p=p->left;
    }
    return p;
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::innerRemove(Node* to_find) {

    Node *parent = innerFind(*to_find);
    Node* found = nullptr;
    if(parent->right == to_find){
        found=parent->right;
    }else if(parent->left == to_find){
        found=parent->left;
    }else{
        //Does not exist
        throw std::exception();
    }
    if (found->isLeaf()) {
        if (parent == nullptr) {
            root = nullptr;
        } else {
            if (parent->right == found) {
                parent->right = nullptr;
            } else {
                parent->left = nullptr;
            }
        }
        delete found;
        return parent;
    } else if (found->isOnlySingleChild()) {
        if (found->right != nullptr) {
            if (parent->right == found) {
                parent->right = found->right;
            } else {
                parent->left = found->right;
            }
        } else {
            if (parent->right == found) {
                parent->right = found->left;
            } else {
                parent->left = found->left;
            }
        }
        found->left->parent = parent;
        delete found;
        return parent;
    } else {
        Node *new_node = findSequential(found);
        Node *new_left_son = found->left;
        Node *new_right_son = found->right;
        Node *new_parent = found->parent;

        if (found == root) {
            root = new_node;
        }
        found->left = new_node->left;
        found->right = new_node->right;
        found->updateHeight();
        found->parent = new_node->parent;

        new_node->left = new_left_son;
        new_node->right = new_right_son;
        new_node->updateHeight();
        new_node->parent = new_parent;
        innerRemove(new_node);
    }
    return nullptr; //shouldnt get here
}

template <class T>
void AVLTree<T>::remove(int key_primary, int key_secondary){
    Node to_find = Node();
    to_find.key_primary = key_primary;
    if (use_secondary_key) {
        to_find.key_secondary = key_secondary;
    } else to_find.key_secondary = 0;
    Node* parent = innerRemove(&to_find);
    while(parent!=nullptr){
        parent->updateHeight();
        int current_bf = parent->getBf();
        if(current_bf==2){
            Node* left_son = parent->left;
            int left_bf = left_son->getBf();
            if(left_bf>=0){
                if(parent->parent== nullptr)
                    root=left_son;
                else{
                    parent->parent->left = left_son;
                }
                rightRotation(parent, left_son);
                //LL
            }
            else{
                Node *left_right_son = left_son->right;
                parent->left = (left_right_son);
                leftRotation(left_son, left_right_son);

                if(parent->parent== nullptr){
                    root=left_right_son;
                }
                else{
                    if(parent->parent->left == parent)
                    {
                        parent->parent->left=left_right_son;
                    }else{
                        parent->parent->right = left_right_son;
                    }
                }
                rightRotation(parent, left_right_son);
                //LR
            }
        }
        if(current_bf==-2){
            Node *right_son = parent->right;
            int right_bf = right_son->getBf();
            if (right_bf <= 0) {
                //RR
                if(parent->parent== nullptr)
                    root=right_son;
                else{
                    parent->parent->right = right_son;
                }
                leftRotation(parent, right_son);
            } else {

                Node *right_left_son = right_son->left;
                parent->right = (right_left_son);
                rightRotation(right_son, right_left_son);

                if (parent->parent == nullptr) {
                    root = right_left_son;
                } else {
                    if(parent->parent->left == parent){
                        parent->parent->left = right_left_son;
                    }else{
                        parent->parent->right = right_left_son;
                    }

                }
                leftRotation(parent, right_left_son);
                //RL
            }
        }
        parent=parent->parent;
    }
    updateBiggest();
}

template<class T>
bool AVLTree<T>::isEmpty() const {
    return root == nullptr;
}

template<class T>
void AVLTree<T>::rightRotation(Node *current_root, Node *root_left_son) {
    Node *temp = root_left_son->right;
    root_left_son->right = (current_root);
    root_left_son->parent=current_root->parent;
    current_root->parent=root_left_son;
    current_root->left =temp;
    if(temp!= nullptr)
        temp->parent=current_root;
    current_root->updateHeight();
    root_left_son->updateHeight();
}

template<class T>
void AVLTree<T>::leftRotation(Node *current_root, Node *root_right_son) {
    Node *temp = root_right_son->left;
    root_right_son->left = (current_root);
    root_right_son->parent=current_root->parent;
    current_root->parent=root_right_son;
    current_root->right =temp;
    if(temp!= nullptr)
        temp->parent=current_root;
    current_root->updateHeight();
    root_right_son->updateHeight();
}

template<class T>
bool  AVLTree<T>::Node::operator==(const AVLTree<T>::Node& to_compare) const {
    return this->key_primary == to_compare.key_primary &&
           this->key_secondary == to_compare.key_secondary;
}

template<class T>
bool AVLTree<T>::Node::operator>(const typename AVLTree<T>::Node &to_compare) const {
    if (this->key_primary == to_compare.key_primary) {
        return this->key_secondary < to_compare.key_secondary;
    }
    return this->key_primary > to_compare.key_primary;
}
template<class T>
void AVLTree<T>::updateBiggest(){
    Node* tmp = root;
    while(tmp!=nullptr){
        biggest = tmp;
        tmp = tmp->right;
    }
}

template<class T>
void AVLTree<T>::deleteTree(){
    treeClear(root);
    root= nullptr;
    biggest= nullptr;
}
template<class T>
void AVLTree<T>::treeClear(Node* root) {
    if(root== nullptr){
        return;
    }
    treeClear(root->left);
    treeClear(root->right);
    delete root;
}

///////////////////////////////////////////////ADDING PRINT FUNCTION JUST FOR OUR USE////////////////////////\
// Helper function to print branches of the binary tree

// Recursive function to print a binary tree.
// It uses the inorder traversal.
template<class T>
void AVLTree<T>::printTree(typename AVLTree<T>::Node* root, Trunk *prev, bool isLeft)
{
    if (root == nullptr) {
        return;
    }

    std::string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);

    printTree(root->right, trunk, true);

    if (!prev) {
        trunk->str = "---";
    }
    else if (isLeft)
    {
        trunk->str = "---";
        prev_str = "   |";
    }
    else {
        trunk->str = "---";
        prev->str = prev_str;
    }

    trunk->showTrunks(trunk);
    std::cout << std::to_string(root->key_primary) << std::endl;

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";

    printTree(root->left, trunk, false);
}

template<class T>
void AVLTree<T>::print() {
    printTree(root, nullptr,false);
}

template<class T>
const T& AVLTree<T>::getBiggest() const  {
    return biggest->data;
}
///////////////////////////////////////////////NEED TO REMEMBER TO DELETE THIS///////////////////////////////
#endif //HW1_MIVNEY_AVLTREE_H