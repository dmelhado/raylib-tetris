#include <raylib.h>
#include <random>
#include <vector>
#include <algorithm>

#include "tetromino.h"

using namespace std;

vector<Color> ColorTable = {{ 15, 15, 15,255} , RED, YELLOW, ORANGE, BLUE, GREEN, {0,255,255,255}, MAGENTA};

void renderSquare(pair<int, int> position, Color color) {
    DrawRectangle(10 + position.first * 25, 10 + position.second * 25, 20, 20, color);
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

    InitWindow(400, 515, "Raylib Tetris");

    //render variables
    const int gameFps = 60;
    SetTargetFPS(gameFps);

    //time variables
    const int boardWidth = 10;
    const int boardHeight = 20;

    int framesPerLine = 48;

    double ticksPerSecond = gameFps / framesPerLine;
    double tickTime = 1 / ticksPerSecond;

    //delayed auto shift
    int DAS_Counter = 0;

    //gameplay variables
    int level = 1;
    int score = 0;

    int singles = 0;
    int doubles = 0;
    int triples = 0;
    int tetris = 0;


    bool gameOver = false;

    vector<vector<int>> playfield(boardWidth,vector<int>(boardHeight,0));

    //player variables
    //For many things, it will be convenient to treat the piece type as a number
    //random bag of next pieces
    vector<int> pieceBag = {};

    for(int i = 1; i <= 7; i++){
        pieceBag.push_back(i);
    }

    shuffle(pieceBag.begin(), pieceBag.end(), std::mt19937(std::random_device()()));

    int currentShapeIndex = 0;
    int nextShapeIndex = 1;

    tetromino movingPiece(Shape(pieceBag[currentShapeIndex]), playfield);

    while (!WindowShouldClose()){

        if(!gameOver){

            if(IsKeyDown(KEY_DOWN)){
                if(DAS_Counter == 0 || DAS_Counter >= 16 && (DAS_Counter - 16) % 6 == 0){
                    movingPiece.move(DOWN);
                }
                DAS_Counter++;
            }
            if(IsKeyDown(KEY_LEFT)){
                if(DAS_Counter == 0 || DAS_Counter >= 16 && (DAS_Counter - 16) % 6 == 0){
                    movingPiece.move(LEFT);
                }
                DAS_Counter++;
            }
            if(IsKeyDown(KEY_RIGHT)){
                if(DAS_Counter == 0 || DAS_Counter >= 16 && (DAS_Counter - 16) % 6 == 0){
                    movingPiece.move(RIGHT);
                }
                DAS_Counter++;
            }
            if(IsKeyPressed(KEY_UP)){
                //hard drop
                for(int i = 0; i < 20; i++){
                    movingPiece.move(DOWN);
                }
            }
            if(IsKeyPressed(KEY_Z)){
                movingPiece.rotate(1);
            }
            if(IsKeyPressed(KEY_X)){
                movingPiece.rotate(-1);
            }

            if(IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_DOWN)){
                DAS_Counter = 0;
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
                        playfield[coord.first][coord.second] = pieceBag[currentShapeIndex];
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
                    //score points!!
                    switch (completeLines) {
                        default:
                            break;
                        case 1:
                            score += 100 * level;
                            break;
                        case 2:
                            score += 200 * level;
                            break;
                        case 3:
                            score += 400 * level;
                            break;
                        case 4:
                            score += 800 * level;
                            break;

                    }

                    //prepare a new moving piece
                    movingPiece.~tetromino();
                    currentShapeIndex = nextShapeIndex;
                    nextShapeIndex++;
                    if(nextShapeIndex > 6){
                        //generate a new random set
                        random_shuffle(pieceBag.begin(), pieceBag.end());
                        nextShapeIndex = 0;
                    }

                    new(&movingPiece) tetromino(Shape(pieceBag[currentShapeIndex]), playfield);
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

        //render everything
        BeginDrawing();
        ClearBackground(BLACK);
        //background board
        for(int i = 0; i < boardWidth; i++){
            for(int j = 0; j < boardHeight; j++){
                renderSquare(make_pair(i,j), ColorTable[playfield[i][j]]);
            }
        }
        //moving piece
        for(auto &coordinate : movingPiece.publishCoordinates()){
            renderSquare(coordinate, ColorTable[pieceBag[currentShapeIndex]]);
        }
        DrawText("SCORE", 325, 15, 20, WHITE);
        DrawText(to_string(score).c_str(), 270, 40, 30, WHITE);
        DrawText("LEVEL", 325, 80, 20, WHITE);
        DrawText(to_string(level).c_str(), 270, 105, 30, WHITE);
        /*DrawText("Singles", 300, 145, 20, WHITE);
        DrawText(to_string(singles).c_str(), 270, 170, 20, WHITE);
        DrawText("Doubles", 300, 220, 20, WHITE);
        DrawText(to_string(doubles).c_str(), 270, 245, 20, WHITE);
        DrawText("Triples", 300, 285, 20, WHITE);
        DrawText(to_string(doubles).c_str(), 270, 310, 20, WHITE);*/
        //draw next piece
        DrawText("NEXT", 270, 370, 40, WHITE);
        switch(pieceBag[nextShapeIndex]){
            case 1:
                renderSquare(make_pair(11,17),ColorTable[1]);
                renderSquare(make_pair(12,17),ColorTable[1]);
                renderSquare(make_pair(13,17),ColorTable[1]);
                renderSquare(make_pair(14,17),ColorTable[1]);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;

        }


        EndDrawing();
        tickTime -= GetFrameTime();
    }
    return 0;
}
