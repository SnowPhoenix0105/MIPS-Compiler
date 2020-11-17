
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                void main()
{
    int a[3][2] = { {1, 2}, {3, 4}, {5, 6}};
    int buf;
    buf = a[0][0];
    printf(buf);
    buf = a[0][1];
    printf(buf);
    buf = a[1][0];
    printf(buf);
    buf = a[1][1];
    printf(buf);
    buf = a[2][0];
    printf(buf);
    buf = a[2][1];
    printf(buf);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                