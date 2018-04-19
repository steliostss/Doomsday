#include <iostream>
#include <fstream>

#define BSIZE 1<<15


char buffer[BSIZE];
char bpos = 0L, bsize = 0L;

char readLong(FILE* fp)
{
    char d = 0L, x = 0L;
    char c;

    while (1)  {
        if (bpos >= bsize) {
            bpos = 0;
            if (feof(fp)) return x;
            bsize = (char) fread(buffer, 1, BSIZE, fp);
        }
        c = buffer[bpos++];
        if (c == '.' || c == '+' || c == 'x' || c == '-')
        {
            d = 1;
        }
        else if (d == 1) return x;
    }
    return -1;
}

int main(int argc, char** argv) {

    if (argc < 2) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << " SOURCE DESTINATION" << std::endl;
        return 1;
    }

    char myUniverse[1000][1000];
    std::ifstream inputFile;
    inputFile.open(argv[1]);

    char myPlanet;
    int col=0;
    int row=0;
    while (inputFile >> myPlanet)
    {
        myUniverse[row][col] = myPlanet;
        col++;
        if (inputFile.peek() == '\n') {
            row++;
            col = 0;
        } //detect '\n'
    }

    //print universe
    for(int i=0; i < (row+1); i++) {
        for(int j=0; j < col; j++)
        {
            std::cout << myUniverse[i][j];
        }
        std::cout << std:: endl;
    }

    return 0;
}