#include <iostream>
#include <fstream>
#include <queue>

static const char plus = '+';
static const char minus = '-';
static const char dot = '.';
static const char obs = 'x';
static const char star = '*';


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
void replAdd(std::queue <universe> &myQueue, universe** myUniverse, int row, int col, int prevTime, char replColor)
{
    myUniverse[row][col].set_data(replColor);
    myUniverse[row][col].set_timeAdded(prevTime+1);
    myUniverse[row][col].set_row(row);
    myUniverse[row][col].set_col(col);
    myQueue.push(myUniverse[row][col]);
}

void switch_case (std::queue <universe> &myQueue, universe** myUniverse, int row, int col, int prevTime, char replColor, char x)
{
    switch (x)
    {
        case dot:
            replAdd(myQueue, myUniverse, row, col, prevTime, replColor);
            break;
        case minus:
            if (replColor != minus) {
                replAdd(myQueue, myUniverse, row, col, prevTime, star);
                break;
            }
        case plus:
            if (replColor != plus) {
                replAdd(myQueue, myUniverse, row, col, prevTime, star);
            }
        default:
            break;
    }//WEST ELEMENT
}

int main(int argc, char** argv) {
    int Mrows = 1000; //rows
    int Ncols = 1000; //columns
    //must read file to calculate lines and columns

    auto** myUniverse = static_cast<universe **>(malloc(Ncols * sizeof(universe)));
    for(int i=0; i<1000; i++) {
        myUniverse[i] = static_cast<universe *>(malloc(Mrows * sizeof(universe)));
    }

    //creating universe
    std::string myPlanet;
    int col=0;
    int row=0;

    std::queue <universe> myQueue;

    //create universe
    std::ifstream inputFile;
    inputFile.open(argv[1]);

    while (inputFile.good())
    {
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
        if (data == minus ||  data == plus)
        {
            std::cout << "ADDED\n";
            myQueue.push(myUniverse[row][col]);
        }

        col++;
    }
    //universe created


    std::cout << std::endl;
    for(int i=0; i < (row+1); i++) {
        for(int j=0; j < col; j++)
        {
            std::cout << (myUniverse[i][j]).get_data();
        }
        std::cout << std:: endl;
    }

    int C = 0;
    int R = 0;

    while(!myQueue.empty())
    {
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
        if (R < row-1) {
            char SOUTH = myUniverse[R + 1][C].get_data();
            switch_case(myQueue, myUniverse, R + 1, C, prevTime, replColor, SOUTH);
        }

        //NORTH
        if (R > 1) {
            char NORTH = myUniverse[R - 1][C].get_data();
            switch_case(myQueue, myUniverse, R - 1, C, prevTime, replColor, NORTH);
        }

        //EAST
        if(C < col-1) {
            char EAST = myUniverse[R][C + 1].get_data();
            switch_case(myQueue, myUniverse, R, C + 1, prevTime, replColor, EAST);
        }

        //WEST
        if(C > 1) {
            char WEST = myUniverse[R][C - 1].get_data();
            switch_case(myQueue, myUniverse, R, C - 1, prevTime, replColor, WEST);
        }
    }

    //print universe

    std::cout << std::endl;
    for(int i=0; i < (row+1); i++) {
        for(int j=0; j < col-1; j++)
        {
            std::cout << (myUniverse[i][j]).get_data();
            std::cout << "_";
            std::cout << (myUniverse[i][j]).get_timeAdded();
            std::cout << " ";
        }
        std::cout << std:: endl;
    }

    return 0;
}