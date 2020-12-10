
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                void main() {
    int num;
    scanf(num);
    printf(num / 1);
    scanf(num);
    printf(num / 2);
    scanf(num);
    printf(num / 4);
    scanf(num);
    printf(num / 8);
    scanf(num);
    printf(num / 64);
    scanf(num);
    printf(num / 1024);
    scanf(num);
    printf(num / 1879048192);
    scanf(num);
    printf(num / -1);
    scanf(num);
    printf(num / -2);
    scanf(num);
    printf(num / -4);
    scanf(num);
    printf(num / -8);
    scanf(num);
    printf(num / -64);
    scanf(num);
    printf(num / -1024);
    scanf(num);
    printf(num / -1879048192);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                