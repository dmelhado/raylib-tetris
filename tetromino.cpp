//
// Created by dante on 11/20/21.
//
#include <iostream>

#include "tetromino.h"
#include "vector"

using namespace std;

//constructor
tetromino::tetromino(Shape shape, const vector<vector<int>>& playfield) {
    playfieldReference = playfield;
    switch (shape) {
        case I:
            _box = {{0,1,0,0},
                    {0,1,0,0},
                    {0,1,0,0},
                    {0,1,0,0}};
            _xPos = 3;
            _yPos = -1;
            break;
        case O:
            _box = {{1,1},
                    {1,1}};
            _xPos = 4;
            _yPos = 0;
            break;
        case T:
            _box = {{0,1,0},
                    {0,1,1},
                    {0,1,0}};
            _xPos = 4;
            _yPos = -1;
            break;
        case J:
            _box = {{0,1,0},
                    {0,1,0},
                    {0,1,1}};
            _xPos = 4;
            _yPos = -1;
            break;
        case L:
            _box = {{0,1,1},
                    {0,1,0},
                    {0,1,0}};
            _xPos = 4;
            _yPos = -1;
            break;
        case S:
            _box = {{0,0,1},
                    {0,1,1},
                    {0,1,0}};
            _xPos = 4;
            _yPos = -1;
            break;
        case Z:
            _box = {{0,1,0},
                    {0,1,1},
                    {0,0,1}};
            _xPos = 4;
            _yPos = -1;
            break;
    }
}

//destructor
tetromino::~tetromino() = default;


void tetromino::rotate() {
    vector<vector<int>> rotation = this->_giveRotation();
    if(_validPiece(rotation, _xPos, _yPos)){
        _box = rotation;
    }
}

//generic calculated rotation for now.
//a fixed table should be much faster and accurate
vector<vector<int>> tetromino::_giveRotation() {
    vector<vector<int>> res(_box.size(),vector<int>(_box.size(),0));
    for(int i = 0; i < _box.size(); i++){
        for(int j = 0; j < _box.size(); j++){
            res[j][_box.size() - 1 - i] = _box[i][j];
        }
    }
    return res;
}

//used to check if every (future) piece block is ok
bool tetromino::_validPiece(vector<vector<int>> &piece, int xPos, int yPos) {
    bool res = true;
    vector<pair<int, int>> coordinates = tetromino::_calculateCoordinates(xPos, yPos, piece);

    for(auto & coordinate : coordinates){
        int cX = coordinate.first;
        int cY = coordinate.second;
        //inside bounds
        if(cX < 0 || cX >= playfieldReference.size()){
            res = false;
            break;
        }
        if(cY < 0 || cY >= playfieldReference[0].size()){
            res = false;
            break;
        }
        //doesn't overlap a board placed block
        if (playfieldReference[cX][cY] != 0){
            res = false;
            break;
        }
    }

    return res;
}

//calculates the physical position of every piece block in the playfield
vector<pair<int, int>> tetromino::_calculateCoordinates(int xPos, int yPos, vector<vector<int>> &piece) {
    vector<pair<int,int>> res;
    for(int i = 0; i < piece.size(); i++) {
        for (int j = 0; j < piece.size(); j++) {
            if(piece[i][j] == 1){
                res.emplace_back(xPos + i, yPos + j);
            }
        }
    }
    return res;
}

void tetromino::move(Movement mov) {
    int futureXPos = _xPos;
    int futureYPos = _yPos;
    switch (mov) {
        case LEFT:
            futureXPos--;
            break;
        case RIGHT:
            futureXPos++;
            break;
        case DOWN:
            futureYPos++;
            break;
    }
    if(_validPiece(_box,futureXPos,futureYPos)){
        _xPos = futureXPos;
        _yPos = futureYPos;
    }
}

vector<pair<int, int>> tetromino::publishCoordinates() {
    return _calculateCoordinates(_xPos, _yPos, _box);
}

