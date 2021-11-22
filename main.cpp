#include <raylib.h>
#include <tuple>
#include <vector>
#include <map>

#include "tetromino.h"

using namespace std;

void renderSquare(pair<int, int> position, Color color) {
    DrawRectangle(10 + position.first * 25, 95 + position.second * 25, 20, 20, color);
}

void eraseLine(vector<vector<int>> &playfield, int line){
    for(int j = line; j > 0; j--){
        for(int i = 0; i < playfield.size(); i++){
            playfield[i][j] = playfield[i][j - 1];
        }
    }
    for(int i = 0; i < playfield.size(); i++){
        playfield[i][0] = 0;
    }
}

int main() {

    const int boardWidth = 10;
    const int boardHeight = 20;

    SetTargetFPS(30);

    double ticksPerSecond = 4;
    double tickTime = 1 / ticksPerSecond;

    vector<vector<int>> playfield(boardWidth,vector<int>(boardHeight,0));

    //For many things, it will be convenient to treat the piece type as a number
    int pieceNumber = GetRandomValue(0, 6);

    tetromino movingPiece(Shape(pieceNumber), playfield);

    InitWindow(400, 600, "Raylib Tetris");
    while (!WindowShouldClose()){

        if(IsKeyPressed(KEY_DOWN)){
            movingPiece.move(DOWN);
        }
        if(IsKeyPressed(KEY_LEFT)){
            movingPiece.move(LEFT);
        }
        if(IsKeyPressed(KEY_RIGHT)){
            movingPiece.move(RIGHT);
        }
        if(IsKeyPressed(KEY_UP)){
            movingPiece.rotate();
        }

        if(tickTime < 0){
            //check if can't move anymore
            bool setPiece = false;
            vector<pair<int,int>> coordinates = movingPiece.publishCoordinates();
            for(auto & coord : coordinates){
                //is on floor
                if(coord.second + 1 >= boardHeight){
                    setPiece = true;
                    break;
                }
                //has a placed block below
                if(playfield[coord.first][coord.second + 1] != 0){
                    setPiece = true;
                    break;
                }
            }
            if(!setPiece){
                movingPiece.move(DOWN);
            }
            else{
                //this guard does almost everything
                //set every block to the board
                for(auto & coord : coordinates){
                    playfield[coord.first][coord.second] = 1;
                }
                //check complete lines
                int completeLines = 0;
                int j = boardHeight - 1;
                while(j >= 0){
                    int sum = 0;
                    for(int i = 0; i < boardWidth; i++){
                        sum += playfield[i][j];
                    }
                    if(sum == boardWidth){
                        completeLines++;
                        eraseLine(playfield, j);
                    }
                    else{
                        j--;
                    }
                }

                movingPiece.~tetromino();
                new(&movingPiece) tetromino(Shape(GetRandomValue(0, 6)), playfield);
            }
            tickTime = 1 / ticksPerSecond;
        }

        BeginDrawing();
        //background board
        for(int i = 0; i < boardWidth; i++){
            for(int j = 0; j < boardHeight; j++){
                Color color = LIGHTGRAY;
                if(playfield[i][j] == 1){
                    color = DARKGRAY;
                }
                renderSquare(make_pair(i,j), color);
            }
        }
        //demo piece
        for(auto &coordinate : movingPiece.publishCoordinates()){
            renderSquare(coordinate, DARKGRAY);
        }

        ClearBackground(WHITE);
        EndDrawing();
        tickTime -= GetFrameTime();
    }
    return 0;
}
