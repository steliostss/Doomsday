#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

static bool boomDoom = false;
static int theTimeHasCome = INT32_MAX;
class universe;

static queue <universe> myQueue;
universe** myUniverse;

class universe {
private:
    char data;
    bool hasMaterial;
    bool hasAntiMaterial;

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

    bool get_hasMaterial () { return hasMaterial; }
    void set_hasMaterial (bool fl) { hasMaterial = fl; }

    bool get_hasAntiMaterial () { return hasMaterial; }
    void set_hasAntiMaterial (bool fl) { hasAntiMaterial = fl; }
};

//replace data and add to Queue
void replAdd(universe &nextUniverse, int curTime, char replColor)
{
    nextUniverse.set_data(replColor);
    nextUniverse.set_timeAdded(curTime+1);

    myQueue.push(nextUniverse);
}

void switch_case (universe &curUniverse, universe &nextUniverse)
{
    char nextState = nextUniverse.get_data();
    int nextTime = nextUniverse.get_timeAdded();

    int curTime = curUniverse.get_timeAdded();
    char replColor = curUniverse.get_data();

    if(boomDoom) {
        if (curTime == theTimeHasCome)
            return;
    }

    switch (nextState)
    {
        case '.':
            replAdd(nextUniverse, curTime, replColor);
            break;
        case '+':
            if (replColor == '-') {
                if(nextTime == curTime){
                    replAdd(curUniverse, curTime, '*');
                }
                replAdd(nextUniverse, curTime, '*');
                boomDoom = true;
                theTimeHasCome = curTime+1;
                break;
            }
            break;
        case '-':
            if (replColor == '+') {
                if(nextTime == curTime){
                    replAdd(nextUniverse, curTime, '*');
                }
                replAdd(curUniverse, curTime, '*');
                boomDoom = true;
                theTimeHasCome = curTime+1;
                break;
            }
            break;
        case '*':
//            if ((replColor == '+' || replColor == '-') && curTime != nextTime)
//            {
//                replAdd(nextUniverse, curTime, '*');
//            }
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
}

void universeTraveler (universe** myUniverse, int &Ncols, int &Mrows)
{
    int C = 0;
    int R = 0;

    while (!myQueue.empty()) {
        universe tempPlanet = myQueue.front();
        //do work
        C = tempPlanet.get_col();
        R = tempPlanet.get_row();
        myQueue.pop();

        //when adding something to queue increment timeAdded by time of previous item+1
        //timeAdded=(time_prev+1)

        int prevTime = myUniverse[R][C].get_timeAdded();

        if(boomDoom) {
            if (prevTime > theTimeHasCome)
                return;
        }
        if (R < Mrows - 1) {
            switch_case(myUniverse[R][C], myUniverse[R+1][C]);
        }//SOUTH

        if (R > 0) {
            switch_case(myUniverse[R][C], myUniverse[R-1][C]);
        }//NORTH

        if (C < Ncols - 1) {
            switch_case(myUniverse[R][C], myUniverse[R][C+1]);
        }//EAST

        if (C > 0) {
            switch_case(myUniverse[R][C], myUniverse[R][C-1]);
        }//WEST
    }
}

void read_universe(universe** myUniverse, char** argv, const int &Ncols, const int &Mrows)
{
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
//                printf("ADDED\n");
                myQueue.push(myUniverse[row][col]);
            }
        }
    }
}

void printUniverse (universe** myUniverse, const int Ncols, const int Mrows)
{
    for (int i = 0; i < Mrows; i++) {
        for (int j = 0; j < Ncols; j++) {
            printf("%c", myUniverse[i][j].get_data());
            //printf("_%d ", myUniverse[i][j].get_timeAdded());
        }
        printf("\n");
    }
    printf("\n");
}

void deleteUniverse (universe** myUniverse, int Mrows)
{
    for (int i = 0; i < Mrows; ++i) {
        delete myUniverse[i];
    }
    delete myUniverse;
}

int main(int argc, char** argv) {
    //must read file to calculate lines and columns
    int Mrows = -1; //rows
    int Ncols = -1; //columns
    read_size_of_universe(argv[1], &Mrows, &Ncols);

    //allocating universe
    auto **myUniverse = new universe*[Mrows];
    for (int i = 0; i < Mrows; ++i) {
        myUniverse[i] = new universe[Ncols];
    }

    //creating universe
    read_universe(myUniverse, argv, Ncols, Mrows);

    //traveling universe
    universeTraveler(myUniverse, Ncols, Mrows);

    //doomsday or not??
    if(boomDoom) {
        printf("%d\n", theTimeHasCome);
    }
    else {
        printf("the world is saved\n");
    }

    //printing universe
    printUniverse(myUniverse, Ncols, Mrows);

    //deleting universe
    deleteUniverse(myUniverse, Mrows);

    return 0;
}