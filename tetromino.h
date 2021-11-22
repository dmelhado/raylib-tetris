//
// Created by dante on 11/20/21.
//

#ifndef RAYLIB_TETRIS_TETROMINO_H
#define RAYLIB_TETRIS_TETROMINO_H

#include "vector"
#include "tuple"

enum Shape{I,O,T,J,L,S,Z};
enum Movement{LEFT,RIGHT,DOWN};

using namespace std;

class tetromino {
public:
    tetromino(Shape shape, const vector<vector<int>>& playfield);
    ~tetromino();
    void rotate();
    void move(Movement mov);
    vector<pair<int,int>> publishCoordinates();

private:
    bool _validPiece(vector<vector<int>> &piece, int xPos, int yPos);
    static vector<pair<int,int>> _calculateCoordinates(int xPos, int yPos, vector<vector<int>> &piece);
    vector<vector<int>> _giveRotation();


    int _xPos;
    int _yPos;
    vector<vector<int>> _box;
    vector<vector<int>> playfieldReference;

};


#endif //RAYLIB_TETRIS_TETROMINO_H
