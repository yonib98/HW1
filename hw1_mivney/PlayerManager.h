#ifndef HW1_MIVNEY_PLAYERMANAGER_H
#define HW1_MIVNEY_PLAYERMANAGER_H

#include "AVLTree.h"
#include "memory"
using std::shared_ptr;
using std::make_shared;
class Player{
    int level;
    int player_id;
};
class Group{
    int group_id;
    AVLTree<shared_ptr<Player>> players_id_tree;
    AVLTree<shared_ptr<Player>> players_level_tree;
};
class PlayerManager{
    AVLTree<shared_ptr<Player>> all_players_id_tree;
    AVLTree<shared_ptr<Player>> all_players_level_tree;
    AVLTree<shared_ptr<Group>> all_groups_tree;
    AVLTree<shared_ptr<Group>> not_empty_groups_tree;
public:
    PlayerManager();
};
#endif //HW1_MIVNEY_PLAYERMANAGER_H
