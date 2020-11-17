
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
    int i;
    int res = 0;
    for (i = 0; i < 3; i = i + 1) {
        res = res + i;
    }
    printf(res);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                