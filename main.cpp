#include <iostream>
#include "AVLTree.h"
#include "memory"
using std::shared_ptr;
struct Player{
    int level;
    int player_id;
};
Player* createPlayer(){
    static int id = 1;
    Player *a = new Player();
    a->level=4;
    a->player_id=id++;
    return a;
}
int main(){
    AVLTree<shared_ptr<Player>> id_tree = AVLTree<shared_ptr<Player>>(false);
    for (int i=1;i<=5;i++){
        Player *tmp = createPlayer();
        shared_ptr<Player> data = shared_ptr<Player>(tmp);
        id_tree.insert(tmp->player_id,tmp->level,data);
    }
    int x;
    return 0;
}
