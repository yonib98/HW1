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
void Group::Player::resetGroup(){
    belong_group=nullptr;
}
shared_ptr<Group> Group::Player::getGroup() const{
    return belong_group;
}
void Group::Player::setLevel(int new_level){
    level=new_level;
}
const AVLTree<std::shared_ptr<Group::Player>>& Group::getPlayersByLevels() const{
    return this->players_level_tree;
}
int Group::getPlayersCount() const{
    return players_level_tree.getSize();
}
void Group::updateBelongs(shared_ptr<Group> ptr) {
    int group_size= getPlayersCount();
    if(group_size==0){
        return;
    }
    shared_ptr<Player>* group_players = new shared_ptr<Player>[group_size];
    players_level_tree.exportToArray(group_players);
    for(int i=0;i<group_size;i++){
        group_players[i]->belong_group=ptr;
    }
    delete[] group_players;

}
Group::Group(int group_id): group_id(group_id), players_id_tree(false), players_level_tree(true){
}

Group::~Group(){
    players_id_tree.deleteTree();
    players_level_tree.deleteTree();
}

int Group::getId() const {
    return group_id;
}
int Group::getGroupSize() const{
    return players_id_tree.getSize();
}
bool Group::isEmpty() const {
    return players_id_tree.isEmpty();
}
void Group::mergeGroups(Group& group_to_merge) {
    this->players_id_tree.mergeWith(group_to_merge.players_id_tree);
    this->players_level_tree.mergeWith(group_to_merge.players_level_tree);

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
void Group::updateGroupBiggest(){
    this->players_id_tree.updateBiggest();
    this->players_level_tree.updateBiggest();
}
PlayerManager::PlayerManager(): all_players_id_tree(false),
                                all_players_level_tree(true),
                                all_groups_tree(false),
                                not_empty_groups_best_players_tree(false){}


void PlayerManager::addPlayer(int player_id, int group_id, int level) {
    //Search for group - log k
    shared_ptr<Group> belong_group = this->all_groups_tree.find(group_id,0);
    shared_ptr<Group::Player> new_player=make_shared<Group::Player>(player_id,level,belong_group);

    all_players_id_tree.insert(player_id,0,new_player);
    all_players_level_tree.insert(level,player_id,new_player);
    belong_group->insertPlayer(new_player);
    if(belong_group->getGroupSize()-1>0) {
        not_empty_groups_best_players_tree.remove(group_id, 0);
    }
    belong_group->updateGroupBiggest();
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
    not_empty_groups_best_players_tree.remove(belong_group->getId(),0);
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
    belong_group->updateGroupBiggest();
    not_empty_groups_best_players_tree.insert(belong_group->getId(),0,belong_group->getBiggest());
}

int PlayerManager::getHighestLevel(int group_id){
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

int PlayerManager::getGroupSize(int group_id){
    if(group_id<0){
        return this->all_players_id_tree.getSize();
    }
    else {
        shared_ptr<Group> group=this->all_groups_tree.find(group_id,0);
        return group->getPlayersCount();
    }
}
PlayerManager::~PlayerManager(){
    //Log n: n_k1+n_k2+...n_kl=n
    int players_count = all_players_id_tree.getSize();
    shared_ptr<Group::Player>* players = new std::shared_ptr<Group::Player>[players_count];
    all_players_id_tree.exportToArray(players);
    shared_ptr<Group::Player>* tmp = players;
    for(int i=0;i<players_count;i++){
        tmp[i]->resetGroup();
    }
    delete[] players;
}

void PlayerManager::getAllPlayersByLevel(int group_id,int* players, int* num_of_players) const {
    if(group_id<0){
        *num_of_players=all_players_level_tree.getSize();
        if(*num_of_players==0){
            throw NoPlayers();
        }
        all_players_level_tree.exportToIDArray(players);
    }else{
        shared_ptr<Group> group = all_groups_tree.find(group_id,0);
        *num_of_players=group->getPlayersCount();
        if(*num_of_players==0){
            throw NoPlayers();
        }
        group->getPlayersByLevels().exportToIDArray(players);
    }
}

void PlayerManager::getGroupsHighestLevel(int numOfGroups, int* players) const {
    funcObj test = funcObj(numOfGroups,players);
    int numOfGroups_copy=numOfGroups;
    if(not_empty_groups_best_players_tree.getSize()<numOfGroups){
        throw NotEnoughGroups();
    }
    not_empty_groups_best_players_tree.inOrder(&numOfGroups_copy,test);
}

void PlayerManager::replaceGroup(int group_id, int replacement_id) {
    //Find gruop to remove and group to replace 2logk
    shared_ptr<Group> to_remove_group = all_groups_tree.find(group_id,0);
    shared_ptr<Group> replacement_group = all_groups_tree.find(replacement_id,0);
    if(to_remove_group->isEmpty()){
       all_groups_tree.remove(group_id,0);
       return;
    }
    not_empty_groups_best_players_tree.remove(group_id,0);
    if(!replacement_group->isEmpty()){
        not_empty_groups_best_players_tree.remove(replacement_id,0);
    }
    //merge: players_group_remove_tree with players of group to replace
    replacement_group->mergeGroups(*to_remove_group);
    replacement_group->updateBelongs(replacement_group);
    all_groups_tree.remove(group_id,0);
    replacement_group->updateGroupBiggest();
    not_empty_groups_best_players_tree.insert(replacement_id,0,replacement_group->getBiggest());
}