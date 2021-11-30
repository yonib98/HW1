#include <iostream>
#include "AVLTree.h"
#include "memory"
using std::shared_ptr;
using std::make_shared;
struct Player{
    int level;
    int player_id;
public:
    Player(int player_id,int level):player_id(player_id), level(level) {};
};
void removeToLL(){
    AVLTree<shared_ptr<Player>> id_tree = AVLTree<shared_ptr<Player>>(false);
    shared_ptr<Player> p1 = make_shared<Player>(7,7);
    shared_ptr<Player> p2 = make_shared<Player>(6,6);
    shared_ptr<Player> p3 = make_shared<Player>(8,8);
    shared_ptr<Player> p4 = make_shared<Player>(5,5);
    id_tree.insert(7,7,p1);
    id_tree.insert(6,6,p2);
    id_tree.insert(8,8,p3);
    id_tree.insert(5,5,p4);
    id_tree.remove(8,8);
}void removeToLR(){
    //Test LR for deletion
    AVLTree<int> tree = AVLTree<int>(false);
    tree.insert(13,0,0);
    tree.insert(8,0,0);
    tree.insert(15,0,0);
    tree.insert(14,0,0);
    tree.insert(17,0,0);
    tree.insert(5,0,0);
    tree.insert(10,0,0);
    tree.insert(6,0,0);
    tree.print();
    tree.remove(10,0);
    tree.print();
}
AVLTree<int> createLecutreTree(){
    AVLTree<int> tree = new AVLTree<int>(false);
    tree.insert(5,0,0);
    tree.insert(2,0,0);
    tree.insert(8,0,0);
    tree.insert(4,0,0);
    tree.insert(6,0,0);
    tree.insert(10,0,0);
    tree.insert(1,0,0);
    tree.insert(3,0,0);
    tree.insert(7,0,0);
    tree.insert(9,0,0);
    tree.insert(12,0,0);
    tree.insert(11,0,0);
    return tree;
}
int main(){
//    removeToLR(); //Does not work
    //what happens: the BF is wrong on left son -> left son of parents needs to be updated
    //Conclusion: parent->parent son is determined by in which tree the BF was bad.
    //Another demo:

    //RL like lecture AVL - the rotation comes from left son there for parent->parent->left is updated
    AVLTree<int> tree = createLecutreTree();
    std::cout << "AVL like lecture before rotation" << std::endl;
    tree.print();
    std::cout << "Afer rotation" << std::endl;
    tree.remove(1,0);
    tree.print();
    tree.deleteTree();

    //RL  like lecture, the rotation comes for RIGHT son, there we have bug.
    AVLTree<int> second_tree = createLecutreTree();
    std::cout << "AVL like lecture before rotation" << std::endl;
    second_tree.print();
    second_tree.remove(9,0);
    std::cout << "Afer rotation" << std::endl;
    second_tree.print();
    tree.deleteTree();

    return 0;
}
