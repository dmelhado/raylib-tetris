#include <raylib.h>
#include <random>
#include <vector>
#include <algorithm>

#include "tetromino.h"

using namespace std;

vector<Color> ColorTable = {{ 15, 15, 15,255} , RED, YELLOW, ORANGE, BLUE, GREEN, {0,255,255,255}, MAGENTA};

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
    //int pieceNumber = GetRandomValue(1, 7);

    //random set of next pieces
    vector<int> setA = {};

    for(int i = 1; i <= 7; i++){
        setA.push_back(i);
    }

    shuffle(setA.begin(),setA.end(), std::mt19937(std::random_device()()));

    int currentShapeIndex = 0;
    int nextShapeIndex = 1;

    tetromino movingPiece(Shape(setA[currentShapeIndex]), playfield);

    bool gameOver = false;

    InitWindow(400, 600, "Raylib Tetris");
    while (!WindowShouldClose()){

        if(!gameOver){

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
                movingPiece.rotate(1);
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
                        playfield[coord.first][coord.second] = setA[currentShapeIndex];
                    }
                    //check complete lines
                    int completeLines = 0;
                    int j = boardHeight - 1;
                    while(j >= 0){
                        int sum = 0;
                        for(int i = 0; i < boardWidth; i++){
                            sum += (playfield[i][j] > 0);
                        }
                        if(sum == boardWidth){
                            completeLines++;
                            eraseLine(playfield, j);
                        }
                        else{
                            j--;
                        }
                    }
                    //prepare a new moving piece
                    movingPiece.~tetromino();
                    currentShapeIndex = nextShapeIndex;
                    nextShapeIndex++;
                    if(nextShapeIndex > 6){
                        //generate a new random set
                        random_shuffle(setA.begin(),setA.end());
                        nextShapeIndex = 0;
                    }

                    new(&movingPiece) tetromino(Shape(setA[currentShapeIndex]), playfield);
                    //check if new piece is overlapping and set game over
                    coordinates = movingPiece.publishCoordinates();
                    for(auto & coord : coordinates){
                        if(playfield[coord.first][coord.second] != 0){
                            gameOver = true;
                        };
                    }
                }
                tickTime = 1 / ticksPerSecond;
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);
        //background board
        for(int i = 0; i < boardWidth; i++){
            for(int j = 0; j < boardHeight; j++){
                renderSquare(make_pair(i,j), ColorTable[playfield[i][j]]);
            }
        }
        //demo piece
        for(auto &coordinate : movingPiece.publishCoordinates()){
            renderSquare(coordinate, ColorTable[setA[currentShapeIndex]]);
        }
        EndDrawing();
        tickTime -= GetFrameTime();
    }
    return 0;
}
