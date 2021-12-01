#include "PlayerManager.h"
int main(){
    PlayerManager a = PlayerManager();
    a.addGroup(2);
    a.addGroup(3);
    a.addGroup(4);
    a.addPlayer(1,2,1);
    a.addPlayer(2,3,4);
    a.addPlayer(3,4,5);
    int numOfGroups = 3;
    std::shared_ptr<Group::Player>* players = new std::shared_ptr<Group::Player>[3];
    a.getGroupsHighestLevel(&numOfGroups,players);
    delete[] players;
    a.replaceGroup(2,3);
    return 0;
}