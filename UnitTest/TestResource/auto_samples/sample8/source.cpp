
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                

char single_char()
{
    return('1');
}

void print_all(int a, char b)
{
    printf(a);
    printf(b);
}

void bigger(int a, int b)
{
    if (a <= b)
    {
        return;
    }
    printf("a is bigger");
}


void main()
{
    char c;
    int i = +123;
    printf("18231045");
    c = single_char();
    ;
    print_all(i, c);
    bigger(324, (c));
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                