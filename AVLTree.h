#ifndef HW1_MIVNEY_AVLTREE_H
#define HW1_MIVNEY_AVLTREE_H
#include <cmath>
#include "memory"
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
        bool operator==(const Node &to_compare) const;

        bool operator>(const Node &to_compare) const;
    };

    bool use_secondary_key;
    Node *root;
    Node *biggest;


    Node *innerFind(const Node &to_search);

    void rightRotation(Node *current_root, Node *root_left_son);

    void leftRotation(Node *current_root, Node *root_right_son);

    void removeNode(Node *);

public:
    AVLTree(bool use_secondary_key);

    Node *find(int key_primary, int key_secondary);

    void insert(int key_primary, int key_secondary, T data);

    bool remove(const Node &v);

    bool isEmpty();
};
template <class T>
AVLTree<T>::AVLTree(bool use_secondary_key): use_secondary_key(use_secondary_key), root(nullptr),biggest(nullptr){}

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
typename AVLTree<T>::Node* AVLTree<T>::find(int key_primary,int key_secondary){
    Node to_search;
    to_search.key_primary = key_primary;
    to_search.key_secondary=key_secondary;
    Node* result = innerFind(to_search);
    if(*result==to_search){
        return result;
    }
    return nullptr;

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
        return;
    }
    if (*result == *to_insert) {
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
                Node *left_son = temp->left;
                int left_bf = left_son->getBf();
                if (left_bf >= 0) {
                    rightRotation(temp, left_son);
                    //LL -> right
                } else {
                    Node *left_right_son = left_son->right;
                    temp->left = (left_right_son);
                    leftRotation(left_son, left_right_son);
                    left_son = temp->left;
                    rightRotation(temp, left_son);
                    //LR
                }
            } else if (current_bf == -2) {
                rotation = true;
                Node *right_son = parent->right;
                int right_bf = right_son->getBf();
                if (right_bf <= 0) {
                    leftRotation(parent, right_son);
                    if(parent->parent== nullptr)
                        root=right_son;
                    else{
                        parent->parent->right = right_son;
                    }
                } else {
                    Node *right_left_son = right_son->left;
                    temp->right = (right_left_son);
                    rightRotation(right_son, right_left_son);
                    right_son = temp->right;
                    leftRotation(temp, right_son);
                }
            }
            temp=parent;
        }
    }


//template <class T>
//void removeNode(Node* v){
//    if(isLeaf(v)){
//
//template <class T>
//void AVLTree:remove(const Node& v){
//
//};
    template<class T>
    bool AVLTree<T>::isEmpty() {
        return root == nullptr;
    }

    template<class T>
    void AVLTree<T>::rightRotation(Node *current_root, Node *root_left_son) {
        Node *temp = root_left_son->right;
        root_left_son->right = (current_root);
        current_root->left = (temp);
    }

    template<class T>
    void AVLTree<T>::leftRotation(Node *current_root, Node *root_right_son) {
        Node *temp = root_right_son->left;
        root_right_son->left = (current_root);
        current_root->right = (temp);
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
#endif //HW1_MIVNEY_AVLTREE_H