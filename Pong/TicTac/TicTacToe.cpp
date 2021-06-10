//
//  TicTacToe.cpp
//  Pong
//
//  Created by Joseph Gu on 6/1/21.
//

#include "TicTacToe.hpp"
//  main.cpp
//  ttt
//
//  Created by Joseph Gu on 4/16/21.
//

#include <iostream>
#include <sstream>
#include "GameAI.hpp"

using namespace std;

void TicTacToe::printgrid (GameGrid& gameGrid) {
    cout << "-------\n";
    cout << "|"<< gameGrid.getElementAt(0,0)<<"|" << gameGrid.getElementAt(0,1) << "|" << gameGrid.getElementAt(0,2) << "| \n";
    cout << "-------\n";
    cout << "|"<< gameGrid.getElementAt(1,0) <<"|" << gameGrid.getElementAt(1,1) << "|" << gameGrid.getElementAt(1,2) << "| \n";
    cout << "-------\n";
    cout << "|"<< gameGrid.getElementAt(2,0) <<"|" << gameGrid.getElementAt(2,1) << "|" << gameGrid.getElementAt(2,2) << "| \n";
    cout << "-------\n";
}

string TicTacToe::fetchPlayerMove () {
    while (true) {
        string input;
        cout << "Make your move: ";
        getline (cin, input);
        if ((input[0] == '1' || input[0] == '2' || input[0] == '3') && (input[1] == 'a' || input[1] == 'b' || input[1] == 'c') && input.length()==2){
            return input;
        } else {
            cout << "You input wrong, try again. \n";
        }
    }
}

int TicTacToe::init() {
    string input;
    int row;
    char column;
    GameGrid gameGrid;
    GameAI gameAI;
    cout << "Welcome to Tic-Tac-Toe! The rows top to bottom are numbered 1-2-3, the columns from left to right are a-b-c. Thus 2b is the middle square \n";
    printgrid (gameGrid);
    while (true) { // check for correct input
        cout << "Enter 1 for 1st move, enter 2 if you want to go 2nd:";
        getline (cin, input);
        if (input == "1") {
            break;
        }
        if (input == "2") {
            gameGrid.updateGrid(2, 'b', 'x');
            printgrid (gameGrid);
            cout << "The program has made its move \n";
            break;
        }
        if ((input != "1")&&(input != "2")) {
            cout << "You input wrong, try again. \n";
        }
    }
    while (true) {
        input = fetchPlayerMove();
        row = int (input[0]) - 48;
        column = input[1];
        gameGrid.updateGrid(row, column,'o');
        printgrid (gameGrid);
        if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getGameOver() == true) {
            if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getNodeValue() == -1) {
                cout << "Game's over. You won. \n";
                return 0;
            }
            if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getNodeValue() == 2) {
                cout << "Game's over. You lost. \n";
                return 0;
            }
            if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getNodeValue() == 1) {
                cout << "Game's over. Tied \n";
                return 0;
            }
        }
        cout << "Press enter to see enemy move:";
        getline (cin, input);
        gameAI.minimax(gameGrid.getReferencetoArray(), true, 1);
        printgrid (gameGrid);
        if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getGameOver() == true) {
            if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getNodeValue() == -1) {
                cout << "Game's over. You won. \n";
                return 0;
            }
            if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getNodeValue() == 2) {
                cout << "Game's over. You lost. \n";
                return 0;
            }
            if ((gameAI.checkGameOver(gameGrid.getReferencetoArray()))->getNodeValue() == 1) {
                cout << "Game's over. Tied. \n";
                return 0;
            }
        }
    }
    return 0;
}
