#include "PlayerManager.h"
using std::shared_ptr;
using std::make_shared;
Group::Player::Player(int player_id, int level, shared_ptr<Group> belong_group): player_id(player_id),level(level),
                                                                                 belong_group(belong_group){
};
int Group::Player::getId() const{
    return player_id;
}
int Group::Player::getLevel() const{
    return level;
}
shared_ptr<Group> Group::Player::getGroup() const{
    return belong_group;
}
void Group::Player::setLevel(int new_level){
    level=new_level;
}
Group::Group(int group_id): group_id(group_id), players_id_tree(false), players_level_tree(true){
}

int Group::getId() const {
    return group_id;
}
bool Group::isEmpty() const {
    return players_id_tree.isEmpty();
}

void Group::insertPlayer(shared_ptr<Player> player_to_add) {
    int player_id= player_to_add->getId();
    int player_level=player_to_add->getLevel();
    this->players_id_tree.insert(player_id,0,player_to_add);
    this->players_level_tree.insert(player_level,player_id,player_to_add);
}

void Group::removePlayer(shared_ptr<Player> player_to_remove){
    int player_id=player_to_remove->getId();
    int player_level=player_to_remove->getLevel();
    this->players_id_tree.remove(player_id,0);
    this->players_level_tree.remove(player_level,player_id);
}
shared_ptr<Group::Player> Group::getBiggest() const{
    return this->players_level_tree.getBiggest();
}
PlayerManager::PlayerManager(): all_players_id_tree(false),
                                all_players_level_tree(true),
                                all_groups_tree(false),
                                not_empty_groups_best_players_tree(false){}


void PlayerManager::addPlayer(int player_id, int group_id, int level) {
    //Search for group - log k
    shared_ptr<Group> belong_group = this->all_groups_tree.find(group_id,0);
    shared_ptr<Group::Player> new_player=make_shared<Group::Player>(player_id,level,belong_group);
    if(!belong_group->isEmpty()){
        //Log k
        not_empty_groups_best_players_tree.remove(group_id,0);
    }
    all_players_id_tree.insert(player_id,0,new_player);
    all_players_level_tree.insert(level,player_id,new_player);
    belong_group->insertPlayer(new_player);
    not_empty_groups_best_players_tree.insert(group_id,0,belong_group->getBiggest());
}

void PlayerManager::addGroup(int group_id){
    shared_ptr<Group> new_group=make_shared<Group>(group_id);
    all_groups_tree.insert(group_id,0,new_group);
}
void PlayerManager::removePlayer(int player_id) {
    //logn
    shared_ptr<Group::Player> player_to_remove = all_players_id_tree.find(player_id,0);
    int player_level=player_to_remove->getLevel();
    //2logn
     all_players_id_tree.remove(player_id,0);
     all_players_level_tree.remove(player_level,player_id);

     //Remove from the group-Log n_k<=n
     shared_ptr<Group> player_group=player_to_remove->getGroup();
     player_group->removePlayer(player_to_remove);
     //LogN
     not_empty_groups_best_players_tree.remove(player_group->getId(),0);
     if(!player_group->isEmpty()){
         not_empty_groups_best_players_tree.insert(player_group->getId(),0,player_group->getBiggest());
     }
}

shared_ptr<Group::Player> PlayerManager::getBestPlayer() const {
    return all_players_level_tree.getBiggest();
}

void PlayerManager::increaseLevel(int player_id, int level_increase){
    shared_ptr<Group::Player> player_to_update = all_players_id_tree.find(player_id,0);
    shared_ptr<Group> belong_group = player_to_update->getGroup();
    //Re-sort trees by levels, id's tree not affected
    int current_level = player_to_update->getLevel();
    //Log n
    all_players_level_tree.remove(current_level,player_id);
    //Log nk
    belong_group->removePlayer(player_to_update);
    int new_level = level_increase+current_level;
    player_to_update->setLevel(new_level);
    //Log n
    all_players_level_tree.insert(new_level,player_id,player_to_update);
    //Log nk
    belong_group->insertPlayer(player_to_update);
}

int PlayerManager::getHighestLevel(int group_id, int player_id){
    if(group_id<0){
        if(all_players_level_tree.isEmpty()){
            return -1;
        }
        return getBestPlayer()->getId();
    }
    shared_ptr<Group> belong_group=all_groups_tree.find(group_id,0);
    if(belong_group->isEmpty()){
        return -1;
    }
    return belong_group->getBiggest()->getId();
}
PlayerManager::~PlayerManager(){
    //Log n: n_k1+n_k2+...n_kl=n
    all_groups_tree.deleteTree();
    all_players_level_tree.deleteTree();
    all_players_id_tree.deleteTree();
    not_empty_groups_best_players_tree.deleteTree();
}