# Board Game

Simple c++ interface to play board game against bots or players.\
Game supported :
- Loot [(french tutorial for "Butin")](http://jeuxstrategieter.free.fr/Butin_complet.php)
- Checkers [(french tutorial for "Dames")](https://infolib.re/storage/files/documents/jeux_de_societe/Les%20dames%20-%20R%C3%A8gle%20du%20jeu.pdf)
- BullTricker [(french tutorial for "BullTricker")](http://www.bulltricker.com/fr/regle_simplifiee.html)

## Table
- Introduction
    - [Demo](#demo)
- Setup
    - [Installation](#installation)
    - [Run](#run)
- Functionality
    - [Features](#features)
    - [Architecture](#architecture)
- Future Development
    - [Future Development](#future-development)

## Demo

### Loot
- Select the number of real players and bot before the party
    - <img height=200 src="resources/demo/loot_menu.png">
- Select a yellow pawn and then click on all the case you want to jump
    - <img height=200 src="resources/demo/loot_begin.png">
- Game is finished when there isn't any action left.
    - <img height=200 src="resources/demo/loot_end.png">

### Checkers

- Choose if you want to play against a bot or a player 
    - <img height=100 src="resources/demo/checkers_menu.png">
- On each turn, the pieces that you can use are marked (in yellow)
    - <img height=200 src="resources/demo/checkers_begin.png">

### BullTrickers

- Choose if you want to play against bot or a player
- On each turn, the pieces that you can use are marked in (yellow)
    - <img height=200 src="resources/demo/bulltricker_game.png">
    

## Setup
### Installation
Compilation require g++ and sfml installed.\
To install sfml :
```bash
# Ubuntu
sudo apt-get update
sudo apt-get install libsfml-dev
# Fedora
sudo dnf install SFML-devel
# Arch linux
sudo pacman -S sfml
```

Compilation
```bash
make
```
Flags
```bash
make RELEASE=1  # to disable debugging.
make MAC=1      # necessary on MAC OS
```

### Run

```bash
./run
```
or
```bash
make run
```

## Features

- Game Loot
- Game Checkers
- Game BullTricker
- Bot
    - RandomStrategy
        - Pick a random action.
    - GreedyStrategy
        - Pick an action the maximise the next turn score.
    - AlphaBetaStrategy
        - Only in zero sum 1vs1 games (checkers and bulltricker).
        - Maximise score over $n$ parties in the future.
            - Assume each player will min/max the player score.

## Architecture

This project has an MVC architecture.

- #### Model :
    - Each game have its own implementation of GameState class
        - a GameState is composed of :
            - a BoardType (inherits Board class)
            - a list of scores
            - its step
            - current player id.
- #### View :
    - The launcher alternate between Menu and Games
        - If Menu.focus is empty, it focus Menu
        - If Menu.focus is non empty, it focus the corresponding Game 
    - Each game have it's own implementation of Game class
        - a Game class is a view for a specific game.
- #### Controller :
    - Each game have it's own implementation of Manager class, Action class
    - Manager acts as the controller :
        - it initialize the first game state, check if an action is valid before applying it. 
    - ActionType :
        - it require to implements the following methods
            - ``getActions`` : Get all the actions appliable on a given GameState.
                - Checkers queen can have a gigantic number of actions possibles, so we restrict the actions given for capture.
            - ``hasRemainingActions`` : is there any action left ?
            - ``isValid`` : Can the action be applied on a GameState ?for
            - ``apply`` : Apply the action onto a GameState.
            - ``actionEquivalence`` : Doe the two actions have the same effect over a given GameState
            

## Future Development

- GUI
    - helper for checker
    - arrows for checker
- BOT
    - MonteCarloStrategy