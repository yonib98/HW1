#include "library1.h"
#include "PlayerManager.h"
#include "Exceptions.h"
//InvalidInput->Alloc error -> failure

void* Init(){
    PlayerManager *DS;
    try{
         DS=new PlayerManager();
    }
    catch(std::bad_alloc& e){
        return NULL;
    }
    return (void*)DS;
}

StatusType AddGroup(void* DS, int GroupID){
    if(DS==NULL || GroupID<=0) {
        return INVALID_INPUT;
    }
    try{
        PlayerManager* PM = (PlayerManager*)DS;
        PM->addGroup(GroupID);
    }
    catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch(AlreadyExists& e){
        return FAILURE;
    }
    return SUCCESS;

}

StatusType AddPlayer(void* DS, int PlayerID, int GroupID, int Level){
    if(DS==NULL || PlayerID<=0 || GroupID<=0 || Level<0){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* PM= (PlayerManager*)DS;
        PM->addPlayer(PlayerID,GroupID,Level);
    }
    catch (std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch (DoesNotExist& e){
        return FAILURE;
    }
    catch(AlreadyExists& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemovePlayer(void* DS, int PlayerID){
    if(DS==NULL || PlayerID<=0){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* PM= (PlayerManager*)DS;
        PM->removePlayer(PlayerID);
    }
    catch(DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType ReplaceGroup(void* DS, int GroupID, int ReplacementID){
    if(DS==NULL || GroupID<=0 || ReplacementID<=0 || GroupID==ReplacementID){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* PM= (PlayerManager*)DS;
                              PM->replaceGroup(GroupID,ReplacementID);
    }
    catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch(DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType IncreaseLevel(void* DS, int PlayerID, int LevelIncrease){
    if(DS==NULL || PlayerID<=0 || LevelIncrease<=0){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* PM= (PlayerManager*)DS;
        PM->increaseLevel(PlayerID,LevelIncrease);
    }
    catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch(DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetHighestLevel(void* DS, int GroupID, int* PlayerID){
    if(DS==NULL || GroupID==0 || PlayerID==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* PM= (PlayerManager*)DS;
        *PlayerID=PM->getHighestLevel(GroupID);
    }
    catch (DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetAllPlayersByLevel(void* DS, int GroupID, int** Players, int* numOfPlayers){
    if(DS==NULL || Players==NULL || numOfPlayers==NULL || GroupID==0){
        return INVALID_INPUT;
    }
    int* sortedPlayers=NULL;
    try{
        PlayerManager* PM= (PlayerManager*)DS;
        int group_size = PM->getGroupSize(GroupID);
        sortedPlayers=(int*)malloc(sizeof(int)*group_size);
        *(Players)=sortedPlayers;
        *numOfPlayers=group_size;
        PM->getAllPlayersByLevel(GroupID,sortedPlayers,numOfPlayers);
    }
    catch(std::bad_alloc& e){
        if(sortedPlayers!=NULL){
            free(sortedPlayers);
        }
        return ALLOCATION_ERROR;
    }
    catch(DoesNotExist& e){
        if(sortedPlayers!=NULL){
            free(sortedPlayers);

        }
        return FAILURE;
    }
    catch(NoPlayers& e){
        if(sortedPlayers!=NULL){
            free(sortedPlayers);
        }
        *(Players)=NULL;
    }
    return SUCCESS;
}

StatusType GetGroupsHighestLevel (void *DS, int numOfGroups, int **Players){
    if(DS==NULL || Players==NULL || numOfGroups<1){
        return INVALID_INPUT;return ALLOCATION_ERROR;
    }
    int* sortedPlayers=NULL;
    try{
        PlayerManager* PM= (PlayerManager*)DS;
        sortedPlayers=(int*)malloc(sizeof(int)*numOfGroups);
        *Players = sortedPlayers;
        PM->getGroupsHighestLevel(numOfGroups,sortedPlayers);
    }
    catch (std::bad_alloc& e){
        if(sortedPlayers!=NULL){
           free(sortedPlayers);
        }
        return ALLOCATION_ERROR;
    }
    catch (NotEnoughGroups& e){
        if(sortedPlayers!=NULL){
            free(sortedPlayers);
        }
        return FAILURE;
    }
    return SUCCESS;
}
void Quit(void** DS){
    if (DS==NULL) {
        return;
    }
    PlayerManager* PM= ((PlayerManager*)*DS);
    delete PM;
    *DS=NULL;
}