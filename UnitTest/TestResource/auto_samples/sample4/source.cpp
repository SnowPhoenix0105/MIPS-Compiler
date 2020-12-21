
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                
int mod(int a, int b) {
    while (a >= b) {
        a = a - b;
    }
    while (a < 0) {
        a = a + b;
    }
    return(a);
}

void main() {
    int N[2][3];
    int tmp, x, y;
    scanf(tmp);
    N[0][0] = mod(tmp, 10);
    scanf(tmp);
    N[0][1] = mod(tmp, 10);
    scanf(tmp);
    N[0][2] = mod(tmp, 10);
    scanf(tmp);
    N[1][0] = mod(tmp, 10);
    scanf(tmp);
    N[1][1] = mod(tmp, 10);
    scanf(tmp);
    N[1][2] = mod(tmp, 10);
    scanf(x);
    scanf(y);
    x = mod(x, 2);
    y = mod(y, 3);
    switch (N[x][y])
    {
break;
    case 0:
    {
        printf(0);
    }
break;
    case 4:
    {
        printf(4);
    }
break;
    case 5:
    {
        printf(5);
    }
break;
    case 6:
    {
        printf(6);
    }
break;
    case 7:
    {
        printf(7);
    }
break;
    case 8:
    {
        printf(8);
    }
break;
    default:
        printf("default");
    }
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                