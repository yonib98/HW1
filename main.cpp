#include "PlayerManager.h"
int main(){
    PlayerManager a = PlayerManager();
    a.addGroup(2);
    a.addGroup(3);
    a.addGroup(4);
    a.addPlayer(1,2,1);
    a.addPlayer(2,3,4);
    a.addPlayer(3,4,5);
    a.addPlayer(4,4,6);
    a.addPlayer(5,2,1);
    int numOfGroups = 3;
    int* players = new int[3];
    a.getGroupsHighestLevel(numOfGroups,players);
    delete[] players;
    a.replaceGroup(2,3);
    a.replaceGroup(3,4);
    a.removePlayer(5);


    return 0;
}