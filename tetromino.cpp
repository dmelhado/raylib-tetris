//
// Created by dante on 11/20/21.
//
#include <iostream>

#include "tetromino.h"
#include "vector"

using namespace std;

//constructor
tetromino::tetromino(Shape shape, const vector<vector<int>> &playfield) {
    playfieldReference = playfield;
    _rotation = 0;
    switch (shape) {
        case I:
            _pieceRotations = 2;
            _Piece = {  {{0, 1, 0, 0},
                         {0, 1, 0, 0},
                         {0, 1, 0, 0},
                         {0, 1, 0, 0}
                        },
                        {{0, 0, 0, 0},
                         {0, 0, 0, 0},
                         {1, 1, 1, 1},
                         {0, 0, 0, 0}
                        }
                    };
            _xPos = 3;
            _yPos = -1;
            break;
        case O:
            _pieceRotations = 1;
            _Piece = {  {{1, 1},
                         {1, 1}
                        }
                     };
            _xPos = 4;
            _yPos = 0;
            break;
        case T:
            _pieceRotations = 4;
            _Piece = {  {{0, 1, 0},
                         {0, 1, 1},
                         {0, 1, 0}
                        },
                        {{0, 1, 0},
                         {1, 1, 1},
                         {0, 0, 0}
                        },
                        {{0, 1, 0},
                         {1, 1, 0},
                         {0, 1, 0}
                        },
                        {{0, 0, 0},
                         {1, 1, 1},
                         {0, 1, 0}
                        }
                     };
            _xPos = 4;
            _yPos = -1;
            break;
        case J:
            _pieceRotations = 4;
            _Piece = {  {{0, 1, 0},
                         {0, 1, 0},
                         {0, 1, 1}
                        },
                        {{0, 0, 1},
                         {1, 1, 1},
                         {0, 0, 0}
                        },
                        {{1, 1, 0},
                         {0, 1, 0},
                         {0, 1, 0}
                        },
                        {{0, 0, 0},
                         {1, 1, 1},
                         {1, 0, 0}
                        },
                     };
            _xPos = 4;
            _yPos = -1;

            break;
        case L:
            _pieceRotations = 4;
            _Piece = {  {{0, 1, 1},
                         {0, 1, 0},
                         {0, 1, 0}
                        },
                        {{1, 0, 0},
                         {1, 1, 1},
                         {0, 0, 0}
                        },
                        {{0, 1, 0},
                         {0, 1, 0},
                         {1, 1, 0}
                        },
                        {{0, 0, 0},
                         {1, 1, 1},
                         {0, 0, 1}
                        },
                     };
            _xPos = 4;
            _yPos = -1;
            break;
        case S:
            _pieceRotations = 2;
            _Piece = {  {{0, 0, 1},
                         {0, 1, 1},
                         {0, 1, 0}
                        },
                        {{1, 1, 0},
                         {0, 1, 1},
                         {0, 0, 0}
                        },
                     };
            _xPos = 4;
            _yPos = -1;
            break;
        case Z:
            _pieceRotations = 2;
            _Piece = {  {{0, 1, 0},
                         {0, 1, 1},
                         {0, 0, 1}
                        },
                        {{0, 0, 0},
                         {0, 1, 1},
                         {1, 1, 0}
                        },
                     };
            _xPos = 4;
            _yPos = -1;
            break;
        case N:
            _Piece = {};
            _xPos = 0;
            _yPos = 0;
            break;
    }
}

//destructor
//not using weird structures so the default destructor is enough
tetromino::~tetromino() = default;

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
    if (_validPiece(_rotation, futureXPos, futureYPos)) {
        _xPos = futureXPos;
        _yPos = futureYPos;
    }
}

void tetromino::rotate(int spin) {
    int futureRotation = _rotation + spin;
    if( futureRotation == -1){
        futureRotation = _pieceRotations - 1;
    }
    if( futureRotation == _pieceRotations){
        futureRotation = 0;
    }
    if (_validPiece(futureRotation, _xPos, _yPos)) {
        _rotation = futureRotation;
    }
}

//used to check if every (future) piece block is ok
bool tetromino::_validPiece(int rotationPos, int xPos, int yPos) {
    bool res = true;
    vector<pair<int, int>> coordinates = tetromino::_calculateCoordinates(xPos, yPos, _Piece[rotationPos]);

    for (auto &coordinate: coordinates) {
        int cX = coordinate.first;
        int cY = coordinate.second;
        //inside bounds
        if (cX < 0 || cX >= int(playfieldReference.size())) {
            res = false;
            break;
        }
        if (cY >= int(playfieldReference[0].size())){
            res = false;
            break;
        }
        //doesn't overlap a board placed block
        if (playfieldReference[cX][cY] != 0) {
            res = false;
            break;
        }
    }

    return res;
}

//calculates the physical position of every piece block in the playfield
vector<pair<int, int>> tetromino::_calculateCoordinates(int xPos, int yPos, vector<vector<int>> &piece) {
    vector<pair<int, int>> res;
    for (int i = 0; i < piece.size(); i++) {
        for (int j = 0; j < piece.size(); j++) {
            if (piece[i][j] == 1) {
                res.emplace_back(xPos + i, yPos + j);
            }
        }
    }
    return res;
}

vector<pair<int, int>> tetromino::publishCoordinates() {
    return _calculateCoordinates(_xPos, _yPos, _Piece[_rotation]);
}

