#include <raylib.h>
#include <random>
#include <vector>

#include "tetromino.h"

using namespace std;

vector<Color> ColorTable = {{15, 15, 15, 255}, RED, YELLOW, ORANGE, BLUE, GREEN, {0, 255, 255, 255}, MAGENTA};

void renderSquare(pair<int, int> position, Color color) {
    DrawRectangle(10 + position.first * 25, 10 + position.second * 25, 20, 20, color);
}

void eraseLine(vector<vector<int>> &playfield, int line) {
    for (int j = line; j > 0; j--) {
        for (auto &i: playfield) {
            i[j] = i[j - 1];
        }
    }
    for (auto &i: playfield) {
        i[0] = 0;
    }
}

//modify this function to adjust level speed
//should avoid using expensive functions
int levelGravity(int level){
    return max(-int(22 * cbrt(level + 1)) + 69, 1);
}

int main() {

    InitWindow(400, 515, "Raylib Tetris");

    //render variables
    const int gameFps = 60;
    SetTargetFPS(gameFps);

    //time variables
    const int boardWidth = 10;
    const int boardHeight = 20;

    //measured in frames per line. the smaller, the faster
    int gravity = 48;

    double ticksPerSecond = double(gameFps) / double(gravity);
    double tickTime = 1 / ticksPerSecond;

    //delayed auto shift
    int DAS_Counter = 0;

    //gameplay variables
    int level = 0;
    int score = 0;

    int lines = 0;
    int singles = 0;
    int doubles = 0;
    int triples = 0;
    int tetris = 0;


    bool gameOver = false;

    vector<vector<int>> playfield(boardWidth, vector<int>(boardHeight, 0));

    //player variables

    int currentShape = GetRandomValue(1, 7);
    int nextShape = GetRandomValue(1, 7);

    tetromino movingPiece(Shape(currentShape), playfield);

    while (!WindowShouldClose()) {

        if (!gameOver) {

            if (IsKeyDown(KEY_DOWN)) {
                if (DAS_Counter == 0 || DAS_Counter >= 16 && (DAS_Counter - 16) % 3 == 0) {
                    movingPiece.move(DOWN);
                }
                DAS_Counter++;
            }
            if (IsKeyDown(KEY_LEFT)) {
                if (DAS_Counter == 0 || DAS_Counter >= 16 && (DAS_Counter - 16) % 3 == 0) {
                    movingPiece.move(LEFT);
                }
                DAS_Counter++;
            }
            if (IsKeyDown(KEY_RIGHT)) {
                if (DAS_Counter == 0 || DAS_Counter >= 16 && (DAS_Counter - 16) % 3 == 0) {
                    movingPiece.move(RIGHT);
                }
                DAS_Counter++;
            }
            if (IsKeyPressed(KEY_UP)) {
                //hard drop
                for (int i = 0; i < 20; i++) {
                    movingPiece.move(DOWN);
                }
                tickTime = 0;
            }
            if (IsKeyPressed(KEY_Z)) {
                movingPiece.rotate(1);
            }
            if (IsKeyPressed(KEY_X)) {
                movingPiece.rotate(-1);
            }

            if (IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_DOWN)) {
                DAS_Counter = 0;
            }

            if (tickTime < 0) {
                //check if can't move anymore
                bool setPiece = false;
                vector<pair<int, int>> coordinates = movingPiece.publishCoordinates();
                for (auto &coord: coordinates) {
                    //is on floor
                    if (coord.second + 1 >= boardHeight) {
                        setPiece = true;
                        break;
                    }
                    //has a placed block below
                    if (playfield[coord.first][coord.second + 1] != 0) {
                        setPiece = true;
                        break;
                    }
                }
                if (!setPiece) {
                    movingPiece.move(DOWN);
                } else {
                    //this guard does almost everything
                    //set every block to the board
                    for (auto &coord: coordinates) {
                        playfield[coord.first][coord.second] = currentShape;
                    }
                    //check complete lines
                    int completeLines = 0;
                    int j = boardHeight - 1;
                    while (j >= 0) {
                        int sum = 0;
                        for (int i = 0; i < boardWidth; i++) {
                            sum += (playfield[i][j] > 0);
                        }
                        if (sum == boardWidth) {
                            completeLines++;
                            eraseLine(playfield, j);
                        } else {
                            j--;
                        }
                    }
                    lines += completeLines;
                    //score points!!
                    switch (completeLines) {
                        default:
                            break;
                        case 1:
                            score += 40 * (level + 1);
                            singles++;
                            break;
                        case 2:
                            score += 100 * (level + 1);
                            doubles++;
                            break;
                        case 3:
                            score += 300 * (level + 1);
                            triples++;
                            break;
                        case 4:
                            score += 1200 * (level + 1);
                            tetris++;
                            break;
                    }
                    //set level and speed
                    if (completeLines != 0) {
                        level = lines / 10;
                        gravity = levelGravity(level);
                        ticksPerSecond = double(gameFps) / double(gravity);
                    }

                    //prepare a new moving piece
                    movingPiece.~tetromino();
                    currentShape = nextShape;
                    nextShape = GetRandomValue(1, 7);

                    new(&movingPiece) tetromino(Shape(currentShape), playfield);
                    //check if new piece is overlapping and set game over
                    coordinates = movingPiece.publishCoordinates();
                    for (auto &coord: coordinates) {
                        if (playfield[coord.first][coord.second] != 0) {
                            gameOver = true;
                        }
                    }
                }
                tickTime = 1 / ticksPerSecond;
            }
        }

        //render everything
        BeginDrawing();
        ClearBackground(BLACK);
        //background board
        for (int i = 0; i < boardWidth; i++) {
            for (int j = 0; j < boardHeight; j++) {
                renderSquare(make_pair(i, j), ColorTable[playfield[i][j]]);
            }
        }
        //moving piece
        for (auto &coordinate: movingPiece.publishCoordinates()) {
            if (coordinate.second >= 0) {
                renderSquare(coordinate, ColorTable[currentShape]);
            }
        }
        DrawText("SCORE", 325, 15, 20, WHITE);
        DrawText(to_string(score).c_str(), 270, 40, 30, WHITE);
        DrawText("LEVEL", 325, 80, 20, WHITE);
        DrawText(to_string(level).c_str(), 270, 105, 30, WHITE);
        DrawText("LINES", 325, 145, 20, WHITE);
        DrawText(to_string(lines).c_str(), 270, 170, 30, WHITE);
        DrawText("x1:", 270, 220, 20, WHITE);
        DrawText(to_string(singles).c_str(), 310, 220, 20, WHITE);
        DrawText("x2:", 270, 250, 20, WHITE);
        DrawText(to_string(doubles).c_str(), 310, 250, 20, WHITE);
        DrawText("x3:", 270, 280, 20, WHITE);
        DrawText(to_string(triples).c_str(), 310, 280, 20, WHITE);
        DrawText("x4:", 270, 310, 20, WHITE);
        DrawText(to_string(tetris).c_str(), 310, 310, 20, WHITE);


        //draw next piece
        if (!gameOver) {
            DrawText("NEXT", 270, 370, 40, WHITE);
            switch (nextShape) {
                //I
                case 1:
                    renderSquare(make_pair(11, 17), ColorTable[1]);
                    renderSquare(make_pair(12, 17), ColorTable[1]);
                    renderSquare(make_pair(13, 17), ColorTable[1]);
                    renderSquare(make_pair(14, 17), ColorTable[1]);
                    break;
                    //O
                case 2:
                    renderSquare(make_pair(11, 17), ColorTable[2]);
                    renderSquare(make_pair(12, 17), ColorTable[2]);
                    renderSquare(make_pair(11, 18), ColorTable[2]);
                    renderSquare(make_pair(12, 18), ColorTable[2]);
                    break;
                    //T
                case 3:
                    renderSquare(make_pair(11, 17), ColorTable[3]);
                    renderSquare(make_pair(12, 17), ColorTable[3]);
                    renderSquare(make_pair(13, 17), ColorTable[3]);
                    renderSquare(make_pair(12, 18), ColorTable[3]);
                    break;
                    //J
                case 4:
                    renderSquare(make_pair(11, 17), ColorTable[4]);
                    renderSquare(make_pair(11, 18), ColorTable[4]);
                    renderSquare(make_pair(12, 18), ColorTable[4]);
                    renderSquare(make_pair(13, 18), ColorTable[4]);
                    break;
                    //L
                case 5:
                    renderSquare(make_pair(13, 17), ColorTable[5]);
                    renderSquare(make_pair(11, 18), ColorTable[5]);
                    renderSquare(make_pair(12, 18), ColorTable[5]);
                    renderSquare(make_pair(13, 18), ColorTable[5]);
                    break;
                    //S
                case 6:
                    renderSquare(make_pair(12, 17), ColorTable[6]);
                    renderSquare(make_pair(13, 17), ColorTable[6]);
                    renderSquare(make_pair(11, 18), ColorTable[6]);
                    renderSquare(make_pair(12, 18), ColorTable[6]);
                    break;
                case 7:
                    renderSquare(make_pair(11, 17), ColorTable[7]);
                    renderSquare(make_pair(12, 17), ColorTable[7]);
                    renderSquare(make_pair(12, 18), ColorTable[7]);
                    renderSquare(make_pair(13, 18), ColorTable[7]);
                    break;
                default:
                    break;

            }
        } else {
            DrawText("GAME\nOVER", 270, 370, 40, WHITE);
        }

        EndDrawing();
        tickTime -= GetFrameTime();
    }
    return 0;
}
