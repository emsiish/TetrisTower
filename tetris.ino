#include <LedControl.h>

#define ROWS 32
#define COLS 32

const int dataInPin1 = 3;
const int clkPin1 = 1;
const int loadPin1 = 2;

const int dataInPin2 = 5; 
const int clkPin2 = 6; 
const int loadPin2 = 4;  

const int dataInPin3 = 12; 
const int clkPin3 = 10; 
const int loadPin3 = 11; 

const int dataInPin4 = 9; 
const int clkPin4 = 8; 
const int loadPin4 = 7; 

LedControl matrix1 = LedControl(dataInPin1, clkPin1, loadPin1, 4);
LedControl matrix2 = LedControl(dataInPin2, clkPin2, loadPin2, 4);
LedControl matrix3 = LedControl(dataInPin3, clkPin3, loadPin3, 4);
LedControl matrix4 = LedControl(dataInPin4, clkPin4, loadPin4, 4);

bool shapes[7][4][4] = {
  // I-shape
  {
    {false, false, false, false},
    {true, true, true, true},
    {false, false, false, false},
    {false, false, false, false},
  },
  // J-shape
  {
    {true, false, false, false},
    {true, true, true, false},
    {false, false, false, false},
    {false, false, false, false},
  },
  // L-shape
  {
    {false, true, true, false},
    {false, false, true, false},
    {false, false, true, false},
    {false, false, false, false},
  },
  // O-shape
  {
    {false, false, false, false},
    {false, true, true, false},
    {false, true, true, false},
    {false, false, false, false},
  },
  // S-shape
  {
    {false, true, true, false},
    {true, true, false, false},
    {false, false, false, false},
    {false, false, false, false},
  },
  // T-shape
  {
    {false, true, false, false},
    {true, true, true, false},
    {false, false, false, false},
    {false, false, false, false},
  },
  // Z-shape
  {
    {true, true, false, false},
    {false, true, true, false},
    {false, false, false, false},
    {false, false, false, false},
  },
};

bool currentShape[4][4];
long currPosX = 0, currPosY = 0, prevPosX = 0, prevPosY = 0;

bool gameBoard[ROWS][COLS];

unsigned long randSeed = 0;

uint8_t currentShapeIndex;

int minX = 3, minY = 3, maxX = 0, maxY = 0;

void initMatrices() {
  for(int i = 0; i < 4; i++) {
    matrix1.shutdown(i, false);
    matrix1.setIntensity(i, 4);
    matrix1.clearDisplay(i);

    matrix2.shutdown(i, false);
    matrix2.setIntensity(i, 4);
    matrix2.clearDisplay(i);

    matrix3.shutdown(i, false);
    matrix3.setIntensity(i, 4);
    matrix3.clearDisplay(i);

    matrix4.shutdown(i, false);
    matrix4.setIntensity(i, 4);
    matrix4.clearDisplay(i);
  }
}

void setupGameBoard() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            gameBoard[i][j] = false;
        }
    }
}

void randomSeedGenerator() {
    randSeed = analogRead(0) + analogRead(1) + analogRead(2);
    randomSeed(randSeed);
}

void randomShapeGenerator() {
    currentShapeIndex = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentShape[i][j] = shapes[currentShapeIndex][i][j];
        }
    }
}

void resetPosition() {
    currPosX = 0;
    currPosY = random(2, COLS - 2);
}

void addShapeOnGameBoard() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentShape[i][j]) {
                gameBoard[currPosX + i][currPosY + j] = true;
            }
        }
    }
}

void drawGameOnLED() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
          if(row <= 7 && col <= 7)
            matrix1.setLed(3, col, row, gameBoard[row][col]);
          else if(row <= 15 && col <= 7)
            matrix1.setLed(2, col, row - 8, gameBoard[row][col]);
          else if(row <= 23 && col <= 7)
            matrix1.setLed(1, col, row - 16, gameBoard[row][col]);
          else
            matrix1.setLed(0, col, row - 24, gameBoard[row][col]);
      
          if(row <= 7 && col > 7 && col <= 15)
            matrix2.setLed(3, col - 8, row, gameBoard[row][col]);
          else if(row <= 15 && col > 7 && col <= 15)
            matrix2.setLed(2, col - 8, row - 8, gameBoard[row][col]);
          else if(row <= 23 && col > 7 && col <= 15)
            matrix2.setLed(1, col - 8, row - 16, gameBoard[row][col]);
          else
            matrix2.setLed(0, col - 8, row - 24, gameBoard[row][col]);

          if(row <= 7 && col > 15 && col <= 23)
            matrix3.setLed(3, col - 16, row, gameBoard[row][col]);
          else if(row <= 15 && col > 15 && col <= 23)
            matrix3.setLed(2, col - 16, row - 8, gameBoard[row][col]);
          else if(row <= 23 && col > 15 && col <= 23)
            matrix3.setLed(1, col - 16, row - 16, gameBoard[row][col]);
          else
            matrix3.setLed(0, col - 16, row - 24, gameBoard[row][col]);
      
          if(row <= 7 && col > 23)
            matrix4.setLed(3, col - 24, row, gameBoard[row][col]);
          else if(row <= 15 && col > 23)
            matrix4.setLed(2, col - 24, row - 8, gameBoard[row][col]);
          else if(row <= 23 && col > 23)
            matrix4.setLed(1, col - 24, row - 16, gameBoard[row][col]);
          else
            matrix4.setLed(0, col - 24, row - 24, gameBoard[row][col]);
        }
    }
}

void removeShapeFromGameBoard() {
    for (int i = maxX; i >= minX; i--) {
        for (int j = maxY; j >= minY; j--) {
            if(!gameBoard[prevPosX + i + 1][prevPosY + j])
                gameBoard[prevPosX + i][prevPosY + j] &= !currentShape[i][j];
        }
    }
}

void updateMinMax() {
    minX = minY = 3;
    maxX = maxY = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentShape[i][j]) {
                if (i < minX) minX = i;
                if (i > maxX) maxX = i;
                if (j < minY) minY = j;
                if (j > maxY) maxY = j;
            }
        }
    }
}

void rotateShape() {
    bool tempShape[4][4];
    removeShapeFromGameBoard();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tempShape[i][j] = currentShape[i][j];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentShape[i][j] = tempShape[3 - j][i];
        }
    }
    updateMinMax();
    if (currPosY < -minY) {
        currPosY = -minY;
    }
    else if (currPosY > COLS - maxY - 1) {
        currPosY = COLS - maxY - 1;
    }
}

void moveShape(int joystickX, int joystickY) {
    if (joystickX < 300 && currPosY > - minY) {
        currPosY--;
    }
    else if (joystickX > 700 && currPosY < COLS - maxY - 1) {
        currPosY++;
    }

    if(joystickY < 300)
        rotateShape();
}

void removeRows() {
    for (int row = 0; row < ROWS; row++) {
        bool rowComplete = true;
        for (int col = 0; col < COLS; col++) {
            if (!gameBoard[row][col]) {
                rowComplete = false;
                break;
            }
        }
        if (rowComplete) {
            for (int r = row - 1; r >= 0; r--) {
                for (int c = 0; c < COLS; c++) {
                    gameBoard[r + 1][c] = gameBoard[r][c];
                }
            }
            for (int c = 0; c < COLS; c++) {
                gameBoard[0][c] = false;
            }
        }
    }
}

void gameOver() {
  for (int i = 0; i < 4; i++) {
    matrix1.clearDisplay(i);
    matrix2.clearDisplay(i);
    matrix3.clearDisplay(i);
    matrix4.clearDisplay(i);
  }
}

bool collision() {
    int shapeIndex = random(0, 7);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if (currentShape[i][j] && !currentShape[i + 1][j] &&
                ((currPosX + maxX) >= 31 ||
                gameBoard[currPosX + i + 1][currPosY + j])) {

                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        currentShape[i][j] = shapes[shapeIndex][i][j];

                resetPosition();
                for(int i = 0; i < 4; i++) {
                  for(int j = 0; j < 4; j++) {
                    if(curentShape[i][j] && gameBoard[currPosX + i][currPosY + j]) {
                      gameOver();
                      return false; //game is over
                    }
                  }                                    
                }
                removeRows();
                return true; //game is not over (could be both collision or no)
            }
        }
    }
}



void setup() {
    initMatrices();
    setupGameBoard();
    randomSeedGenerator();
    randomShapeGenerator();
    resetPosition();
    addShapeOnGameBoard();
    drawGameOnLED();
}

void loop() {
    int joystickX = analogRead(A0);
    int joystickY = analogRead(A1);
    updateMinMax();
    moveShape(joystickX, joystickY);

    removeShapeFromGameBoard();

    addShapeOnGameBoard();

    if(!collision())
    {
      return;
    }

    drawGameOnLED();

    prevPosX = currPosX;
    prevPosY = currPosY;

delay(50);
currPosX++;
}
