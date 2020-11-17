
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                int matrix[3][4] = 
{
    { 1,  2,  3,  4},
    { 5,  6,  7,  8},
    { 9, 10, 11, 12}
};

void main()
{
    int r, x, y;
    printf("18231045");
    r = matrix[0][0];
    printf(r);
    r = matrix[0][1];
    printf(r);
    r = matrix[0][2];
    printf(r);
    r = matrix[0][3];
    printf(r);
    r = matrix[1][0];
    printf(r);
    r = matrix[1][1];
    printf(r);
    r = matrix[1][2];
    printf(r);
    printf(matrix[1][3]);
    scanf(x);
    scanf(y);
    while (x >= 3)
    {
        x = x - 3;
    }
    while (x < 0)
    {
        x = x + 3
    }
    while (y >= 4)
    {
        y = y - 4;
    }
    while (y < 0)
    {
        y = y + 4;
    }
    r = matrix[x][y];
    printf(r);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                