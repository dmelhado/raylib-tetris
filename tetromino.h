//
// Created by dante on 11/20/21.
//

#ifndef RAYLIB_TETRIS_TETROMINO_H
#define RAYLIB_TETRIS_TETROMINO_H

#include "vector"
#include "tuple"

enum Shape{N,I,O,T,J,L,S,Z};
enum Movement{LEFT,RIGHT,DOWN};

using namespace std;

class tetromino {
public:
    tetromino(Shape shape, const vector<vector<int>>& playfield);
    ~tetromino();
    void rotate(int spin);
    void move(Movement mov);
    vector<pair<int,int>> publishCoordinates();

private:
    bool _validPiece(int rotationPos, int xPos, int yPos);
    static vector<pair<int,int>> _calculateCoordinates(int xPos, int yPos, vector<vector<int>> &piece);

    int _xPos;
    int _yPos;
    vector<vector<vector<int>>> _Piece;
    int _rotation;
    int _pieceRotations;
    vector<vector<int>> playfieldReference;

};


#endif //RAYLIB_TETRIS_TETROMINO_H
