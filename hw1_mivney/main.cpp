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
void insertTOLR(){
    AVLTree<int> id_tree = AVLTree<int>(false);
    id_tree.insert(13,0,0);
    id_tree.insert(6,0,0);
    id_tree.insert(15,90,0);
    id_tree.insert(5,0,0);
    id_tree.insert(8,0,0);
    id_tree.insert(7,0,0);
    id_tree.deleteTree();
}
void removeToLL(){
    AVLTree<shared_ptr<Player>> id_tree = AVLTree<shared_ptr<Player>>(false);
    shared_ptr<Player> p1 = make_shared<Player>(5,0);
    shared_ptr<Player> p2 = make_shared<Player>(2,0);
    shared_ptr<Player> p3 = make_shared<Player>(8,0);
    shared_ptr<Player> p4 = make_shared<Player>(4,0);
    shared_ptr<Player> p5 = make_shared<Player>(1,0);
    shared_ptr<Player> p6 = make_shared<Player>(6,0);
    shared_ptr<Player> p7 = make_shared<Player>(10,0);
    shared_ptr<Player> p8 = make_shared<Player>(3,0);
    shared_ptr<Player> p9 = make_shared<Player>(7,0);
    shared_ptr<Player> p10 = make_shared<Player>(9,0);
    shared_ptr<Player> p11 = make_shared<Player>(12,0);
    shared_ptr<Player> p12 = make_shared<Player>(11,0);
    id_tree.insert(5,0,p1);
    id_tree.insert(2,0,p2);
    id_tree.insert(8,0,p3);
    id_tree.insert(4,0,p4);
    id_tree.insert(1,0,p5);
    id_tree.insert(6,0,p6);
    id_tree.insert(10,0,p7);
    id_tree.insert(3,0,p8);
    id_tree.insert(7,0,p9);
    id_tree.insert(9,0,p9);
    id_tree.insert(12,0,p9);
    id_tree.insert(11,0,p9);
    id_tree.print();
    id_tree.remove(1,1);
    std::cout<<std::endl << std::endl;
    id_tree.print();
    id_tree.deleteTree();
}
int main(){
    insertTOLR();
    removeToLL();

    int x;
    return 0;
}
