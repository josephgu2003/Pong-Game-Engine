//
//  GameAI.cpp
//  ttt
//
//  Created by Joseph Gu on 4/22/21.
//

#include "GameAI.hpp"

GameStatus* GameAI::checkGameOver (char grid[3][3]) {
    for (int i = 0; i < 3; i++) { //check rows
        if (grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2] )
        {
            if (grid[i][2] == 'o') {
                return new GameStatus(true, -1);
            }
            if (grid[i][2] == 'x') {
                return new GameStatus(true ,2);
        }
    }
    }
    for (int i = 0; i < 3; i++) { // check columns
        if (grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]) {
            if (grid[0][i] == 'o') {
                return new GameStatus(true, -1);
            }
            if (grid[0][i] == 'x') {
                return new GameStatus(true ,2);
        }
        }
    }
    if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
            if (grid[2][2] == 'o') {
                return new GameStatus(true, -1);
            }
            if (grid[2][2] == 'x') {
                return new GameStatus(true, 2);
        }
    }
    if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
            if (grid[1][1] == 'o') {
                return new GameStatus(true, -1);
            }
            if (grid[1][1] == 'x') {
                return new GameStatus(true ,2);
        }
    }
    if (grid[0][0] != ' ' && grid[0][1] != ' ' && grid[0][2] != ' ' && // tie situation
        grid[1][0] != ' ' && grid[1][1] != ' ' && grid[1][2] != ' ' &&
        grid[2][0] != ' ' && grid[2][1] != ' ' && grid[2][2] != ' '
        ) {
        return new GameStatus(true, 1);
    }
    return new GameStatus (false, 3); // return this if the game is not done, hopefully that int of 2 does not get used
}

int GameAI::minimax (char grid[3][3], bool isAI, int step) {
    GameStatus *pGameStatus = checkGameOver(grid); /*check game over, if over, return the terminal node value*/
    static int newR, newC;
    bool gameOver = pGameStatus->getGameOver();
    int value = pGameStatus->getNodeValue();
    delete pGameStatus;
    if (gameOver) {
        return value;
    }
    if (isAI== true) {
        //iterate through possible moves, need coords of unused squares, double for loop, create new grid with move and minimax again
        int maxNode = -2;
        for (int r=0; r<3; r++){
            for (int c=0; c<3; c++) {
            if (grid[r][c] == ' ') {
                char grid2[3][3];
                for (int r2=0; r2<3; r2++) { // for loop copies old grid to new grid
                    for (int c2=0; c2<3; c2++) {
                        grid2[r2][c2] = grid[r2][c2];
                    }
                }
                grid2[r][c] = 'x'; // copies the new node to new grid
                int newNode = minimax (grid2, false, step+1);
                if (newNode > maxNode) {
                    maxNode = newNode;
                    if (step == 1) {
                        newR = r;
                        newC = c;
                    }
                }
        }
            }
        }
        if (step == 1) {
            grid[newR][newC] = 'x';
        }
        return maxNode;
    }
    if (isAI == false) {
        //iterate through possible moves, need coords of unused squares, double for loop, create new grid with move and minimax again
        int minNode = 2;
        for (int r=0; r<3; r++){
            for (int c=0; c<3; c++) {
            if (grid[r][c] == ' ') {
                char grid2[3][3];
                for (int r2=0; r2<3; r2++) { // for loop copies old grid to new grid
                    for (int c2=0; c2<3; c2++) {
                        grid2[r2][c2] = grid[r2][c2];
                    }
                }
                grid2[r][c] = 'o'; // copies the new node to new grid
                int newNode = minimax(grid2, true, step+1);
                minNode = std::min(minNode, newNode);
            }
        }
        }
        return minNode;
    }
    return 0;
}
