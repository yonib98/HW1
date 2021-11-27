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
}
int main(){

    int x;
    return 0;
}
