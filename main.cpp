#include <iostream>
#include <fstream>

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