#include <iostream>
#include <queue>

class test_class {
    private:
        int length = -1;
        int first = -1;
        int height = -1;
    public:
        int getfirst()
        {
            return first;
        }
        int getlength()
        {
            return length;
        }
        void setfirst(int a)
        {
            first = a;
        }
        void setlength(int b)
        {
            length = b;
        }
        void printHello();

        int getheight() { return height; }
        int setheight(int val) { height = val; }


        test_class() {}
        test_class(int len);
        test_class(int len, int fir);
        test_class(int len, int fir, int hei);
        ~test_class() {}
};

test_class::test_class(int len, int fir)
{
    length = len;
    first = fir;
    height = 0;
}

test_class::test_class(int len, int fir, int hei):
    length(len), first(fir), height(hei)
{ }

test_class::test_class(int len):
        length(len)
{ }


void test_class::printHello()
{
    std::cout << "Hello\n";
}

int main(int argc, char** argv) {

    std::cout << "Hello, World!" << std::endl;


    if (argc < 2) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << " SOURCE DESTINATION" << std::endl;
        return 1;
    }

    return 0;
}