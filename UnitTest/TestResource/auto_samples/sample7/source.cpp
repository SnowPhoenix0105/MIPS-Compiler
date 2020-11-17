
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                const int False = -1;

void main()
{
    char c = '+';
    int i,j,k;
    int result;
    printf("18231045");
    printf(c);
    c = '*';
    printf(c);
    i = (c);
    printf("ascii:", i);
    scanf(i);
    scanf(j);
    k = +(i * j);
    printf(k);
    scanf(i);
    scanf(j);
    k = i / j;
    printf(k);
    scanf(i);
    scanf(j);
    scanf(k);
    scanf(c);
    result = i + j * k * c;
    printf(result);
    ;
    result = (i + j) * k * c;
    printf(result);
    result = - (i + j);
    printf(result);
    printf(False);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                