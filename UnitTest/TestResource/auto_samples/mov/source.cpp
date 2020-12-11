
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
    int i, j, k, l, m, n;
    int rlt;
    scanf(i);
    scanf(j);
    scanf(k);
    scanf(l);
    scanf(m);
    scanf(n);
    if (i < j)
    {
        rlt = m;
    }
    else
    {
        rlt = n;
    }
    printf(rlt);
    if (i >= j)
    {
        rlt = m;
    }
    else
    {
        rlt = n;
    }
    printf(rlt);
    if (i < j)
    {
        rlt = m;
    }
    printf(rlt);
    if (i >= j)
    {
        rlt = m;
    }
    printf(rlt);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                