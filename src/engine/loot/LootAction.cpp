#include "engine/loot/LootAction.hpp"

////////////////////////////////////////////////////////////////////////
// STATIC

std::vector<CellPosition> const LootAction::authorizedOffsets = {
    {2, 0}, {0, 2}, {-2, 0}, {0, -2},
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2}
};

/*
    All possibles action from current configuration (up to isomorphism).
*/
std::vector<LootAction> LootAction::getActions(
    const LootManager * manager, 
    PlayerId author, 
    int step,
    Board board) { 
    
    (void)manager; (void)author; (void)step; (void)board; 
    throw NotImplemented(); 
};

/*
    Is there any authorized/correct action ?
*/
bool LootAction::hasRemainingActions(
    const LootManager * manager, 
    PlayerId author, 
    int step,
    Board board) {
     
    (void)manager; (void)author; (void)step; (void)board;
    throw NotImplemented(); 
};

////////////////////////////////////////////////////////////////////////
// STATIC

/*
    Is action authorized/correct.
*/
bool LootAction::isValid(Board) const { 
    throw NotImplemented(); 

};

/*
    If action is not valid, throw an error.
*/
Board LootAction::apply(Board) const { 
    throw NotImplemented(); 
};