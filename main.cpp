#include "PlayerManager.h"
int main(){
    PlayerManager a = PlayerManager();
    a.addGroup(2);
    a.addGroup(3);
    a.addPlayer(1,2,1);
    a.addPlayer(2,3,4);
}