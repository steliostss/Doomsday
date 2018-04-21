

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

class universe {
private:
    char data;
    int timeAdded;

    int row;
    int col;
public:
    char get_data() { return data; }
    void set_data(char c) { data = c; }

    int get_timeAdded() { return timeAdded; }
    void set_timeAdded(int t) { timeAdded = t; }

    int get_row() { return row; }
    void set_row(int r) { row = r; }

    int get_col() { return col; }
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

void switch_case (queue <universe> &myQueue, universe** &myUniverse, const int &row, const int &col, const int &prevTime, const char &replColor, const char &horizonState)
{
    switch (horizonState)
    {
        case '.':
            replAdd(myQueue, myUniverse, row, col, prevTime, replColor);
            break;

        case '+':
            if (replColor == '-') {
                replAdd(myQueue, myUniverse, row, col, prevTime, '*');
                break;
            }
            break;
        case '-':
            if (replColor == '+') {
                replAdd(myQueue, myUniverse, row, col, prevTime, '*');
                break;
            }
            break;
        case '*':
            break;
        case 'X':
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
}

int main(int argc, char** argv) {
    //must read file to calculate lines and columns

//    with malloc
//    auto** myUniverse = static_cast<universe **>(malloc(Ncols * sizeof(universe)));
//    for(int i=0; i<1000; i++) {
//        myUniverse[i] = static_cast<universe *>(malloc(Mrows * sizeof(universe)));
//    }

    int Mrows = 1000; //rows
    int Ncols = 1000; //columns
    read_size_of_universe(argv[1], &Mrows, &Ncols);


    auto **myUniverse = new universe *[Ncols];
    for (int i = 0; i < Ncols; ++i) {
        myUniverse[i] = new universe[Mrows];
    }

    //creating universe
    string myPlanet;
    int col = 0;
    int row = 0;

    queue<universe> myQueue;

    //create universe
    ifstream inputFile;
    inputFile.open(argv[1]);

    //read file
    while (inputFile.good()) {
        auto c = (char) inputFile.get();
        if (c == '\n') {
            row++;
            col = 0;
            continue;
        }
        myUniverse[row][col].set_data(c);
        char data = myUniverse[row][col].get_data();
        myUniverse[row][col].set_row(row);
        myUniverse[row][col].set_col(col);
        //keep row and col because when you push it in the queue you lose it
        //this way you can access the south/north/east/west planet

        //set timeAdded = time = 0
        myUniverse[row][col].set_timeAdded(0);

        //put elements with t=0 to queue
        if (data == '-' || data == '+') {
            printf("ADDED\n");
            myQueue.push(myUniverse[row][col]);
        }

        col++;
    }

    //universe created
    row++;

    printf("\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%c", myUniverse[i][j].get_data());
        }
        printf("\n");
    }

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

        //SOUTH
        if (R < row - 1) {
            char SOUTH = myUniverse[R + 1][C].get_data();
            switch_case(myQueue, myUniverse, R + 1, C, prevTime, replColor, SOUTH);
        }

        //NORTH
        if (R > 1) {
            char NORTH = myUniverse[R - 1][C].get_data();
            switch_case(myQueue, myUniverse, R - 1, C, prevTime, replColor, NORTH);
        }

        //EAST
        if (C < col - 1) {
            char EAST = myUniverse[R][C + 1].get_data();
            switch_case(myQueue, myUniverse, R, C + 1, prevTime, replColor, EAST);
        }

        //WEST
        if (C > 1) {
            char WEST = myUniverse[R][C - 1].get_data();
            switch_case(myQueue, myUniverse, R, C - 1, prevTime, replColor, WEST);
        }
    }

    //print universe

    printf("\n");
    printf("\n");

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 1; j++) {
            printf("%c", myUniverse[i][j].get_data());

        }
        printf("\n");
    }

    printf("\n");

    return 0;

}

