#ifndef HW1_MIVNEY_AVLTREE_H
#define HW1_MIVNEY_AVLTREE_H
#include <cmath>
#include "memory"
#include "string"
#include <iostream>
#include "Exceptions.h"
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

        Node& operator=(const Node& to_copy);
        ~Node(){
            parent=nullptr;
            left=nullptr;
            right=nullptr;
        };

    };
    int size;
    bool use_secondary_key;
    Node *root;
    Node *biggest;


    Node *innerFind(const Node &to_search) const;

    Node* findSequential(Node* p);

    void rightRotation(Node *current_root, Node *root_left_son);

    void leftRotation(Node *current_root, Node *root_right_son);

    Node* innerRemove(Node* to_find);

    Node* sequentialRemove(Node* seq_to_Remove);

    void treeClear(Node* root);

    void printTree(Node* root, Trunk *trunks, bool isLeft);

    void merge(Node** A,int na,Node** B,int nb, Node** C);

    static Node* createCompleteTree(Node* root, int h);

public:
    explicit AVLTree(bool use_secondary_key);

    AVLTree& operator=(const AVLTree& tree_to_copy);

    T& find(int key_primary, int key_secondary) const;

    void insert(int key_primary, int key_secondary, T data);

    void remove(int key_primary, int key_secondary);

    void updateBiggest();

    bool isEmpty() const;

    const T& getBiggest() const;

    void print();

    int getSize() const;

    void helpExport(Node* root,T* arr, int* count) const;

    void helpIDExport(Node* root,int* arr,int *count) const;

    T* exportToArray(T* arr) const;

    int* exportToIDArray(int* arr) const;

    void exportToNodeArray(Node* root,Node** arr,int* count);

    void mergeWith(AVLTree& another_tree);

    static AVLTree almostCompleteTree(bool use_secondary_key,int n);

    void makeAlmostComplete(Node* root,int n);

    void pushArrayToTree(Node* root,Node** arr,int* index);

    void deleteTree();

    Node* treeCopy(Node* root);

    template<class Predicate>
            void inOrder(int* count,Predicate p) const;
    template<class Predicate>
            void helpInOrder(int* count,Predicate p,Node* root) const;

    ~AVLTree();
};
template <class T>
AVLTree<T>::AVLTree(bool use_secondary_key): use_secondary_key(use_secondary_key), root(nullptr),biggest(nullptr),size(0){}

template<class T>
AVLTree<T>::~AVLTree() {
    deleteTree();
}
template<class T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& tree_to_copy){
    if(this==&tree_to_copy){
        return *this;
    }
    this->deleteTree();
   root = this->treeCopy(tree_to_copy.root);
    size=tree_to_copy.size;
    use_secondary_key=tree_to_copy.use_secondary_key;
    biggest=tree_to_copy.biggest;
    return *this;

}

template <class T>
typename AVLTree<T>::Node* AVLTree<T>::innerFind(const Node& to_search) const {
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
T& AVLTree<T>::find(int key_primary,int key_secondary) const {
    Node to_search;
    to_search.key_primary = key_primary;
    to_search.key_secondary=key_secondary;
    Node* result = innerFind(to_search);
    if(result!= nullptr){
        if(*result==to_search){
            return result->data;
        }
    }
    throw DoesNotExist();

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
        size++;
        updateBiggest();
        return;
    }
    if (*result == *to_insert) {
        delete to_insert;
        throw AlreadyExists();
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
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = left_son;
                    } else {
                        parent->parent->right = left_son;
                    }
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
        } else if (current_bf == -2) {
            rotation = true;
            Node *right_son = parent->right;
            int right_bf = right_son->getBf();
            if (right_bf <= 0) {
                //RR
                if(parent->parent== nullptr)
                    root=right_son;
               else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = right_son;
                    } else {
                        parent->parent->right = right_son;
                    }
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
        temp=parent;
    }
    size++;
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

    Node *found = innerFind(*to_find);
    Node* parent;
    if(*found==*to_find){
        parent=found->parent;
    }else{
        throw DoesNotExist();
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
        if(parent==nullptr){
            root=found->right!=nullptr? found->right:found->left;
            root->parent=nullptr;
            delete found;
            return parent;
        }
        if (found->right != nullptr) {
            if (parent->right == found) {
                parent->right = found->right;
                found->right->parent=parent;
            } else {
                parent->left = found->right;
                found->right->parent=parent;
            }
        } else {
            if (parent->right == found) {
                parent->right = found->left;
                found->left->parent=parent;
            } else {
                parent->left = found->left;
                found->left->parent=parent;
            }
        }
        if(found->left!=nullptr){
            found->left->parent = parent;
        }
        delete found;
        return parent;
    } else {
        Node *new_node = findSequential(found);
        if (found == root) {
            root = new_node;
        }
        if(new_node==found->right){
            //No left son for found
            new_node->left=found->left;
            found->left->parent=new_node;
            if(parent!=nullptr){
                parent->right=new_node;
            }
            new_node->parent=parent;
            delete found;
            if(parent== nullptr){
                return new_node;
            }
            return parent;

        }
        Node *new_left_son = found->left;
        Node *new_right_son = found->right;
        Node *new_parent = found->parent;

        if(new_node->right!=nullptr){
            new_node->right->parent=found;
        }
        found->left = nullptr;
        found->right = new_node->right;
        found->updateHeight();
        new_node->parent->left=found;
        found->parent = new_node->parent;

        new_node->left = new_left_son;
        new_node->right = new_right_son;
        new_node->updateHeight();
        new_node->parent = new_parent;

        if(new_left_son!= nullptr){
            new_left_son->parent=new_node;
        }
        if(new_right_son!= nullptr){
            new_right_son->parent=new_node;
        }

       Node* p= sequentialRemove(found);
        return p;
    }
    return nullptr; //shouldnt get here
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::sequentialRemove(Node *seq_to_remove){
    Node* tmp = root->right;
    while(tmp!=nullptr){
        if(*seq_to_remove==*(tmp->left)){
            if(tmp->left->isLeaf()){
                delete tmp->left;
                tmp->left=nullptr;
                return tmp;
            }else{
                Node* to_delete = tmp->left;
                tmp->left=tmp->left->right;
                tmp->left->right->parent=tmp;
                delete to_delete;
                return tmp;
            }
        }
        tmp=tmp->right;
    }
    return nullptr;
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
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = left_son;
                    } else {
                        parent->parent->right = left_son;
                    }
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
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = right_son;
                    } else {
                        parent->parent->right = right_son;
                    }
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
    size--;
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
typename AVLTree<T>::Node& AVLTree<T>::Node::operator=(const Node &to_copy) {
    if(this==&to_copy){
        return *this;
    }
    key_primary= to_copy.key_primary;
    key_secondary=to_copy.key_secondary;
    data=to_copy.data;
    return *this;
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
    biggest=root;
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
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::treeCopy(typename AVLTree<T>::Node* root){
    if(root==nullptr){
        return nullptr;
    }
    typename AVLTree<T>::Node* node =new typename AVLTree<T>::Node();
    *(node)=*root;
    node->left= treeCopy(root->left);
    if(node->left!=nullptr){
        node->left->parent=node;
    }
    node->right= treeCopy(root->right);
    if(node->right!=nullptr){
        node->right->parent=node;
    }
    node->updateHeight();
    return node;
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
template <class T>
void AVLTree<T>::helpIDExport(Node* root,int* arr, int* count) const {
    if(root==nullptr || *count==size){
        return;
    }
    helpIDExport(root->right,arr,count);
    if(root->key_secondary==0){
        arr[*count]=root->key_primary;
    }else{
        arr[*count] = root->key_secondary;
    }
    (*count)++;
    helpIDExport(root->left,arr,count);

}
template<class T>
void AVLTree<T>::helpExport(Node* root,T* arr, int* count) const {
    if(root==nullptr || *count==size){
        return;
    }
    helpExport(root->right,arr,count);
    arr[*count] = root->data;
    (*count)++;
    helpExport(root->left,arr,count);

}
template<class T>
T* AVLTree<T>::exportToArray(T* arr) const {
    int count = 0;
    helpExport(root,arr,&count);
    return arr;
}
template<class T>
int* AVLTree<T>::exportToIDArray(int* arr) const {
    int count = 0;
    helpIDExport(root,arr,&count);
    return arr;
}
template<class T>
template<class Predicate>
void AVLTree<T>::helpInOrder(int* count, Predicate p, typename AVLTree<T>::Node* root) const {
    if(root==nullptr || *count==0) {
        return;
    }
    helpInOrder(count,p,root->left);
    p(root->data,count);
    (*count)--;
    helpInOrder(count,p,root->right);
}
template<class T>
template<class Predicate>
void AVLTree<T>::inOrder(int* count,Predicate p) const {
    helpInOrder(count,p,root);
}
template<class T>
int AVLTree<T>::getSize() const {
    return size;
}
template<class T>
void AVLTree<T>::exportToNodeArray(typename AVLTree<T>::Node* root,
                                             typename AVLTree<T>::Node** arr,int* count) {
    if(root==nullptr || *count==size){
        return;
    }
    exportToNodeArray(root->left,arr,count);
    arr[*count] = root;
    (*count)++;
    exportToNodeArray(root->right,arr,count);
}
template<class T>
void AVLTree<T>::merge(typename AVLTree<T>::Node** A,int na,
                       typename AVLTree<T>::Node** B,int nb, typename AVLTree<T>::Node** C){
    int ia=0,ib=0,ic=0;
    while(ia<na && ib < nb){
        if(*A[ia]>*B[ib]){
           C[ic++]=B[ib++];
        }else{
            C[ic++]=A[ia++];
        }
    }
    for(;ia<na;ic++,ia++){
        C[ic]=A[ia];
    }
    for(;ib<nb;ib++,ic++){
        C[ic]=B[ib];
    }
}
template<class T>
void AVLTree<T>::mergeWith(AVLTree<T>& another_tree) {
    int second_arr_size = another_tree.getSize();
    Node** my_arr = new Node*[size];
    Node** second_arr = new Node*[second_arr_size];
    int count=0;
    this->exportToNodeArray(this->root,my_arr,&count);

    count=0;
    another_tree.exportToNodeArray(another_tree.root,second_arr,&count);

    int merged_arr_size = size+second_arr_size;
    Node** merge_arr = new Node*[merged_arr_size];
    merge(my_arr,size,second_arr,another_tree.getSize(),merge_arr);

    AVLTree<T> almost_complete_tree = AVLTree<T>::almostCompleteTree(this->use_secondary_key,merged_arr_size);
    int index=0;
    almost_complete_tree.pushArrayToTree(almost_complete_tree.root,merge_arr,&index);
    almost_complete_tree.updateBiggest();
    //O(n) do delete  + o(n_k1+nk_2) for copying
    *this=almost_complete_tree;
    delete[] my_arr;
    delete[] second_arr;
    delete[] merge_arr;
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::createCompleteTree(typename AVLTree<T>::Node* root,int h){
     root= new typename AVLTree<T>::Node();
    if(h==0){
        return root;
    }
    root->left=createCompleteTree(root->left,h-1);
    root->left->parent=root;
    root->right=createCompleteTree(root->right,h-1);
    root->right->parent=root;
    root->updateHeight();
    return root;
}

template<class T>
AVLTree<T> AVLTree<T>::almostCompleteTree(bool use_secondary_key,int n){
    AVLTree<T> almost_complete_tree = AVLTree<T>(use_secondary_key);
    int h = std::ceil(std::log2(n+1))-1;
   almost_complete_tree.root = AVLTree<T>::createCompleteTree(nullptr,h);
   almost_complete_tree.size=std::pow(2,h+1)-1;

   almost_complete_tree.makeAlmostComplete(almost_complete_tree.root,n);
   //reverse in order -> almost complete tree
    return almost_complete_tree;
}
template<class T>
void AVLTree<T>::makeAlmostComplete(typename AVLTree<T>::Node* root, int n){
    if (root==nullptr || size==n){
        return;
    }
    if(root->isLeaf()){
        if(root->parent!=nullptr){
            Node* parent = root->parent;
            if(root==parent->left){
                parent->left=nullptr;
            }else{
                parent->right=nullptr;
            }
        }
        delete root;
        size--;
        return;
    }
    makeAlmostComplete(root->right,n);
    makeAlmostComplete(root->left,n);

}
template<class T>
void AVLTree<T>::pushArrayToTree(typename AVLTree<T>::Node* root,typename AVLTree<T>::Node** arr,int* index){
    if(*index==size || root==nullptr){
        return;
    }
    pushArrayToTree(root->left,arr,index);
    *root=*(arr[*index]);
    (*index)++;
    pushArrayToTree(root->right,arr,index);
}
///////////////////////////////////////////////NEED TO REMEMBER TO DELETE THIS///////////////////////////////
#endif //HW1_MIVNEY_AVLTREE_H