#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"

std::vector<CellPosition> const CheckersAction::authorizedOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

/*
    Is there any authorized/correct action ?
*/
bool CheckersAction::hasRemainingActions(
    const CheckersManager *manager, CheckersState state) {

	CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType rivalPawn = state.player == 0 ? CellPieceType::BlackPawn : CellPieceType::WhitePawn;
    CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;
    CellPieceType rivalKing = state.player == 0 ? CellPieceType::BlackKing : CellPieceType::WhiteKing;
     
    /* initialisation of the party */
    if (state.step < manager->players.size())
        return true;
    
    std::vector<CheckersAction> result;
    std::vector<CellPosition> pawns;
    for (int x = 0; x < 10; x++) for (int y = 0; y < 10; y++) {
        if (state.board.getCell(x, y).pieceType == playerPawn || state.board.getCell(x, y).pieceType == playerKing)
            pawns.push_back(CellPosition{x, y});
    }

    for (auto pawn : pawns) for (auto offset : CheckersAction::authorizedOffsets) {
		if (offset.x == 2 || offset.y == -2) {
			CellPosition between = pawn + (offset)/2;
			CellPosition afterJump = pawn + offset;
			
			if (!state.board.isCaseInBoard(between) || !state.board.isCaseInBoard(afterJump))
				continue;
		
			if (!(state.board.isCaseEmpty(between) || state.board.getCell(between).pieceType == playerPawn) &&
				state.board.isCaseEmpty(afterJump))
				return true;
		} else if (offset.x == 1 || offset.y == -1) {
			CellPosition afterJump = pawn + offset;
			if (!state.board.isCaseInBoard(afterJump))
				continue;
			if (state.board.isCaseEmpty(afterJump))
				return true;
		}
    }
    return false;
};

inline bool iff(bool const a, bool const b) {
    return (a && b) || (!a && !b);
};

bool CheckersAction::isValid(CheckersState state) const { 
	if (jumps.size() == 0 || jumps.size() == 1) 
        return false;

    if (!state.board.isCaseInBoard(jumps[0]))
        return false;

    CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType rivalPawn = state.player == 0 ? CellPieceType::BlackPawn : CellPieceType::WhitePawn;
    CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;
    CellPieceType rivalKing = state.player == 0 ? CellPieceType::BlackKing : CellPieceType::WhiteKing;

    if (!(state.board.getCell(jumps[0]) == playerPawn || state.board.getCell(jumps[0]) == playerKing))
        return false;
    if (jumps.size() == 2) {
        if (!state.board.isCaseInBoard(jumps[1]))
            return false;
        CellPosition lastPosition = jumps[0];
        CellPosition currentPosition = jumps[1];
        CellPosition offset = currentPosition - lastPosition;
        if (offset == CellPosition(1,1) || offset == CellPosition(1,-1) || offset == CellPosition(-1,1) || offset == CellPosition(-1,-1)) {
            if (state.board.isCaseEmpty(currentPosition))
                return true;
            return false;
        } else if (offset == CellPosition(2,2) || offset == CellPosition(2,-2) || offset == CellPosition(-2,2) || offset == CellPosition(-2,-2)) {
            CellPosition between = (lastPosition+currentPosition) / 2;
            if (!state.board.isCaseInBoard(between))
                return false;
            if (!(state.board.getCell(between).pieceType == rivalPawn) || !state.board.isCaseEmpty(currentPosition))
                return false;
            return true;
        }
    }
    if (state.board.getCell(jumps[0]) == playerPawn){
        for (long unsigned int i = 0 ; i < jumps.size() - 1 ; i++) {
            CellPosition lastPosition = jumps[i];
            CellPosition currentPosition = jumps[i+1];
            CellPosition offset = currentPosition - lastPosition;
            if (! (offset == CellPosition(2,2) || offset == CellPosition(2,-2) || offset == CellPosition(-2,2) || offset == CellPosition(-2,-2)))
                return false;
            CellPosition between = lastPosition + (offset)/2;
            if (!state.board.isCaseEmpty(currentPosition))
                return false;
            if (!state.board.isCaseInBoard(currentPosition) || !state.board.isCaseInBoard(between))
                return false;
            if (!(state.board.getCell(between).pieceType == rivalPawn || state.board.getCell(between).pieceType == rivalKing))
                return false;
        }
        return true;
    } else {
        std::vector<CellPosition> visited;
        for (long unsigned int i = 0 ; i < jumps.size() - 1 ; i++) {
            CellPosition lastPosition = jumps[i];
            CellPosition currentPosition = jumps[i+1];
            CellPosition offset = currentPosition - lastPosition;
            int xDirection = (offset.x > 0) ? 1 : -1;
            int yDirection = (offset.y > 0) ? 1 : -1;
            if (!(offset.x == offset.y || offset.x == -offset.y))
                return false;
            for (int x = lastPosition.x + xDirection, y = lastPosition.y + yDirection; 
            x != currentPosition.x; 
            x += xDirection, y += yDirection) {
                CellPosition currentCell(x, y);
                CellPosition nextCell(x + xDirection, y + yDirection);

                if (!state.board.isCaseInBoard(currentCell) || !state.board.isCaseInBoard(nextCell))
                    return false;
                if (state.board.getCell(currentCell).pieceType == rivalPawn || state.board.getCell(currentCell).pieceType == rivalKing)
                {
                    if (std::find(visited.begin(), visited.end(), currentCell) != visited.end())
                        return false;
                    if (state.board.getCell(nextCell).pieceType == rivalPawn || state.board.getCell(nextCell).pieceType == rivalKing)
                        return false;
                    visited.push_back(currentCell);
                }
            }
        }
        return true;
    }
    return false;
}

void CheckersAction::removePointsFromScore(Board board, int & score) const {
    for (int x = 0; x < 10; x++) for (int y = 0; y < 10; y++)
        switch (board.getCell(x, y).pieceType) {
            case CellPieceType::WhitePawn : 
                score -= this->manager->WHITE_BONUS; break;
            case CellPieceType::BlackPawn : 
                score -= this->manager->BLACK_BONUS; break;
            case CellPieceType::NoneCell :
                break;
            default : throw NotImplemented();
        }
}

CheckersState CheckersAction::applyPawn(CheckersState state) const{
    auto board = state.board;
    auto scores = state.scores;
    auto cells = board.cellPieces;

    int moveScore = 0;
    CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;
    
    cells[jumps[0].y][jumps[0].x] = CellPiece(CellPieceType::NoneCell);
    if (jumps.size() == 2) {
        CellPosition lastPosition = jumps[0];
        CellPosition currentPosition = jumps[1];
        CellPosition offset = currentPosition - lastPosition;
        if (offset == CellPosition(1,1) || offset == CellPosition(1,-1) || offset == CellPosition(-1,1) || offset == CellPosition(-1,-1)) {
            cells[currentPosition.y][currentPosition.x] = CellPiece(playerPawn);
        } else if (offset == CellPosition(2,2) || offset == CellPosition(2,-2) || offset == CellPosition(-2,2) || offset == CellPosition(-2,-2)) {
            CellPosition between = (lastPosition+currentPosition) / 2;
            cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
            cells[currentPosition.y][currentPosition.x] = CellPiece(playerPawn);
            moveScore += 1;
        }
    } else {
        for (long unsigned int i = 0 ; i < jumps.size() - 1 ; i++) {
            CellPosition lastPosition = jumps[i];
            CellPosition currentPosition = jumps[i+1];
            CellPosition offset = currentPosition - lastPosition;
            CellPosition between = lastPosition + (offset)/2;
            cells[lastPosition.y][lastPosition.x] = CellPiece(CellPieceType::NoneCell);
            cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
            cells[currentPosition.y][currentPosition.x] = CellPiece(playerPawn);
            moveScore += 1;
        }
    }
    if (playerPawn == CellPieceType::WhitePawn && jumps[jumps.size()-1].y == 9)
        cells[jumps[jumps.size()-1].y][jumps[jumps.size()-1].x] = CellPiece(playerKing);
    else if (playerPawn == CellPieceType::BlackPawn && jumps[jumps.size()-1].y == 0)
        cells[jumps[jumps.size()-1].y][jumps[jumps.size()-1].x] = CellPiece(playerKing);

    int nPlayers = manager->players.size();
    int authorIndex = manager->getPlayerIndex(author);

    PlayerId nextPlayer = (
        manager->players[(authorIndex + 1) % nPlayers]
        ).id;
    
    scores[authorIndex] += moveScore;
    
    Board nextBoard{cells, nextPlayer};
    
    CheckersState tempNextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
        };

    if (manager->isFinished(tempNextState)) {
        this->removePointsFromScore(nextBoard, scores[authorIndex]);   
        
    }

    CheckersState nextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
    };

    return nextState;
}

CheckersState CheckersAction::applyKing(CheckersState state) const{
    auto board = state.board;
    auto scores = state.scores;
    auto cells = board.cellPieces;

    int moveScore = 0;
    CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;
    
    cells[jumps[0].y][jumps[0].x] = CellPiece(CellPieceType::NoneCell);
    for (long unsigned int i = 0 ; i < jumps.size() - 1 ; i++) {
        CellPosition lastPosition = jumps[i];
        CellPosition currentPosition = jumps[i+1];
        CellPosition offset = currentPosition - lastPosition;
        int xDirection = (offset.x > 0) ? 1 : -1;
        int yDirection = (offset.y > 0) ? 1 : -1;
        for (int x = lastPosition.x + xDirection, y = lastPosition.y + yDirection; 
        x != currentPosition.x; 
        x += xDirection, y += yDirection) {
            if (state.board.getCell(x, y).pieceType != CellPieceType::NoneCell)
                moveScore += 1;
            cells[y][x] = CellPiece(CellPieceType::NoneCell);            
        }
    }
    cells[jumps[jumps.size()-1].y][jumps[jumps.size()-1].x] = CellPiece(playerKing);

    int nPlayers = manager->players.size();
    int authorIndex = manager->getPlayerIndex(author);

    PlayerId nextPlayer = (
        manager->players[(authorIndex + 1) % nPlayers]
        ).id;
    
    scores[authorIndex] += moveScore;
    
    Board nextBoard{cells, nextPlayer};
    
    CheckersState tempNextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
        };

    if (manager->isFinished(tempNextState)) {
        this->removePointsFromScore(nextBoard, scores[authorIndex]);   
        
    }

    CheckersState nextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
    };

    return nextState;
}

CheckersState CheckersAction::apply(CheckersState state) const {
    CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;
    if (state.board.getCell(jumps[0]).pieceType == playerPawn)
        return applyPawn(state);
    else if (state.board.getCell(jumps[0]).pieceType == playerKing)
        return applyKing(state);
    else
        throw NotImplemented();
}

std::string CheckersAction::toString() const {
    return Cli::toString(jumps);
};