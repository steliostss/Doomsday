

#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;
//
//static const char '+' = '+';
//static const char '-' = '-';
//static const char '.' = '.';
//static const char 'X' = 'X';
//
//static const char '*' = '*';


static bool boomDoom = false;
static int theTimeHasCome = -1;

class universe {
private:
    char data;
    int timeAdded;

    int row;
    int col;
public:
    char get_data() const { return data; }
    void set_data(char c) { data = c; }

    int get_timeAdded() const { return timeAdded; }
    void set_timeAdded(int t) { timeAdded = t; }

    int get_row() const { return row; }
    void set_row(int r) { row = r; }

    int get_col() const { return col; }
    void set_col(int c) { col = c; }
};

//replace data and add to Queue
void replAdd(queue <universe> &myQueue, universe** myUniverse, int row, int col, int prevTime, char replColor)
{
    myUniverse[row][col].set_data(replColor);
    myUniverse[row][col].set_timeAdded(prevTime+1);
    myUniverse[row][col].set_row(row);
    myUniverse[row][col].set_col(col);
    myQueue.push(myUniverse[row][col]);
}

void switch_case (queue <universe> &myQueue, universe** &myUniverse, const int &nextRow, const int &nextCol, const char &nextState, const int &prevTime, const int &curTime, const int &curRow, const int &curCol,  const char &curColor)
{
    switch (nextState)
    {
        case '.':
            replAdd(myQueue, myUniverse, nextRow, nextCol, prevTime, curColor);
            break;

        case '+':
            if (curColor == '-') {
                if(curTime == prevTime){
                    replAdd(myQueue, myUniverse, curRow, curCol, prevTime, '*');
                }
                replAdd(myQueue, myUniverse, nextRow, nextCol, prevTime, '*');
                boomDoom = true;
                theTimeHasCome = prevTime+1;
                break;
            }
            break;
        case '-':
            if (curColor == '+') {
                if(curTime == prevTime){
                    replAdd(myQueue, myUniverse, curRow, curCol, prevTime, '*');
                }
                replAdd(myQueue, myUniverse, nextRow, nextCol, prevTime, '*');
                boomDoom = true;
                theTimeHasCome = prevTime+1;
                break;
            }
            break;
        default:
            break;
    }//WEST ELEMENT
}

void read_size_of_universe(char* argv, int *ROWS, int *COLS)
{

    ifstream inputFile;
    inputFile.open(argv);

    int readfirstLine { 0 };

    int nRow = 0;
    int nCol = 0;
    string line;
    while(getline(inputFile, line))
    {
        if (readfirstLine == 0) {
            nCol = (int) line.length();
            readfirstLine++;
            nRow++;
        }
        else {
            nRow++;
        }
    }
    *ROWS = nRow;
    *COLS = nCol;
    inputFile.close();
    return;
}

void universeTraveler (universe **myUniverse, queue<universe> &myQueue, int &Ncols, int &Mrows)
{
    int C = 0;
    int R = 0;

    //    bool first'*' = false;
    //    int doomTIME = -1;

    while (!myQueue.empty()) {
        universe tempPlanet = myQueue.front();
        //do work
        C = tempPlanet.get_col();
        R = tempPlanet.get_row();
        myQueue.pop();

        //when adding something to queue increment timeAdded by time of previous item+1
        //timeAdded=(time_prev+1)
        char replColor = myUniverse[R][C].get_data();

        int prevTime = myUniverse[R][C].get_timeAdded();

        if (R < Mrows - 1) {
            int curTime = myUniverse[R + 1][C].get_timeAdded();
            char SOUTH = myUniverse[R + 1][C].get_data();
            switch_case(myQueue, myUniverse, R+1, C, SOUTH, prevTime, curTime, R, C, replColor);
        }//SOUTH

        if (R > 0) {
            int curTime = myUniverse[R - 1][C].get_timeAdded();
            char NORTH = myUniverse[R - 1][C].get_data();
            switch_case(myQueue, myUniverse, R - 1, C, NORTH, prevTime, curTime, R, C, replColor);
        }//NORTH

        if (C < Ncols - 1) {
            int curTime = myUniverse[R][C+1].get_timeAdded();
            char EAST = myUniverse[R][C + 1].get_data();
            switch_case(myQueue, myUniverse, R, C + 1, EAST, prevTime, curTime, R, C, replColor);
        }//EAST

        if (C > 0) {
            int curTime = myUniverse[R][C-1].get_timeAdded();
            char WEST = myUniverse[R][C - 1].get_data();
            switch_case(myQueue, myUniverse, R, C - 1, WEST, prevTime, curTime, R, C, replColor);
        }//WEST

        if (boomDoom)
            return;
    }
}


int main(int argc, char** argv) {
    //must read file to calculate lines and columns

    int Mrows = -1; //rows
    int Ncols = -1; //columns
    read_size_of_universe(argv[1], &Mrows, &Ncols);

//    printf("%d %d\n", Mrows, Ncols);


    auto **myUniverse = new universe* [Mrows];
    for (int i = 0; i < Mrows; ++i) {
        myUniverse[i] = new universe[Ncols];
    }

    //creating universe

    queue<universe> myQueue;

    ifstream inputFile;
    inputFile.open(argv[1]);

    for(int row=0; row<Mrows; row++) {
        for (int col=0; col<Ncols; col++) {
            auto c = (char) inputFile.get();
            if (c == '\n'){
                col = -1;
                continue;
            }
            myUniverse[row][col].set_data(c);
            myUniverse[row][col].set_row(row);
            myUniverse[row][col].set_col(col);
            //keep row and col because when you push it in the queue you lose it
            //this way you can access the south/north/east/west planet
            myUniverse[row][col].set_timeAdded(0);

            if (c == '-' || c == '+') {
                //printf("ADDED\n");
                myQueue.push(myUniverse[row][col]);
            }
        }
    }
    //universe created

//    printf("\n");
//    for (int i = 0; i < Mrows; i++) {
//        for (int j = 0; j < Ncols; j++) {
//            printf("%c", myUniverse[i][j].get_data());
//        }
//        printf("\n");
//    }

    universeTraveler(myUniverse, myQueue, Ncols, Mrows);


    //print universe
    if(boomDoom) {
        printf("%d\n", theTimeHasCome);
    }
    else {
        printf("the world is saved\n");
    }
    for (int i = 0; i < Mrows; i++) {
        for (int j = 0; j < Ncols; j++) {
            printf("%c", myUniverse[i][j].get_data());

        }
        printf("\n");
    }

    printf("\n");

    return 0;

}

