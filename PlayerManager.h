#ifndef HW1_MIVNEY_PLAYERMANAGER_H
#define HW1_MIVNEY_PLAYERMANAGER_H

#include "AVLTree.h"
#include "memory"
using std::shared_ptr;
using std::make_shared;

class Group{
public:
    class Player{
        int level;
        int player_id;
        shared_ptr<Group> belong_group;
        friend class Group;
    public:
        Player(int player_id,int level,shared_ptr<Group> belong_group);
        int getId() const;
        int getLevel() const;
        void setLevel(int new_level);
        shared_ptr<Group> getGroup() const;
        void resetGroup();

    };
    Group(int group_id);
    ~Group();
    int getId() const;
    int getGroupSize() const;
    void insertPlayer(shared_ptr<Player> player_to_add);
    void removePlayer(shared_ptr<Player> player_to_remove);
    bool isEmpty() const;
    void mergeGroups(Group& group_to_merge);
    void updateBelongs(shared_ptr<Group> ptr);
    void updateGroupBiggest();
    shared_ptr<Player> getBiggest() const;
    int getPlayersCount() const;
    const AVLTree<std::shared_ptr<Player>>& getPlayersByLevels() const;
private:
    int group_id;
    AVLTree<shared_ptr<Player>> players_id_tree;
    AVLTree<shared_ptr<Player>> players_level_tree;

};
class PlayerManager{
    AVLTree<shared_ptr<Group::Player>> all_players_id_tree;
    AVLTree<shared_ptr<Group::Player>> all_players_level_tree;
    AVLTree<shared_ptr<Group>> all_groups_tree;
    AVLTree<shared_ptr<Group::Player>> not_empty_groups_best_players_tree;
    shared_ptr<Group::Player> getBestPlayer() const;

    void getBiggestPlayer(shared_ptr<Group::Player> player,std::shared_ptr<Group::Player>* players, int index);
public:
    PlayerManager();
    void addPlayer(int player_id,int group_id, int level);
    void addGroup(int group_id);
    void removePlayer(int player_id);
    void replaceGroup(int group_id, int replacement_id);
    void increaseLevel(int player_id, int level_increase);
    int getHighestLevel(int group_id);
    void getAllPlayersByLevel(int group_id,int* Players, int* num_of_players) const;
    void getGroupsHighestLevel(int numOfGroups, int* players) const;
    int getGroupSize(int group_id);
    ~PlayerManager();


    class funcObj{
        int numOfGroups;
        int* players;
    public:
        funcObj(int numOfGroups,int* players): numOfGroups(numOfGroups), players(players){}
        void operator()(shared_ptr<Group::Player> player,int* count){
            players[numOfGroups-*count]=player->getId();
        }
    };
};
#endif //HW1_MIVNEY_PLAYERMANAGER_H