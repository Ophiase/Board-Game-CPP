#include "test/test.hpp"
#include "utils/Cli.hpp"

#include "engine/BoardSided.hpp"

#include <stdlib.h>
#include <iostream>
#include <vector>

int testMain() {
    Cli::info("Test !");

    std::vector<std::vector<CellPiece>> cells(5,
        std::vector<CellPiece>(5, CellPiece{CellPieceType::NoneCell}));


    std::vector<std::vector<SidePiece>> horizontals(
        6, std::vector<SidePiece>(5, SidePiece{SidePieceType::NoneSide}));
    std::vector<std::vector<SidePiece>> verticals(
        5, std::vector<SidePiece>(6, SidePiece{SidePieceType::NoneSide}));

    BoardSided a{cells, horizontals, verticals, 0}; 
    
    SidePosition b{SideVector{0, 0}, true};
    CellPosition c{0, 3};

    Cli::debug(std::to_string(a.isCaseInBoard(b)));
    Cli::debug(std::to_string(a.isCaseInBoard(c)));

    return EXIT_SUCCESS;
}