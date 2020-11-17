
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                



const char c_1 = 'o';
const char c_2 = 'p', c_3 = 'a';
char c_4 = 'b';
char c_array[3] = {'c', 'd', 'e'};
char c_matrix[3][3] = 
{
    {'f', 'g', 'h'},
    {'i', 'j', 'k'},
    {'l', 'm', 'n'}
};


void main()
{
    const int i_1 = 1;
    const int i_2 = 2, i_3 = 3;
    int  i_4 = 4;
    int i_array[3] = {5, 6, 7};
    int i_matrix[3][3] = 
    {
        {8 , 9 , 10},
        {11, 12, 13},
        {14, 15, 16}
    };
    int result;
    int i, j;
    char result_c;
    printf("18231045");
    result = c_1 + c_2 + c_3 + c_4;
    printf(result);
    for (i = 0; i < 3; i = i + 1)
    {
        result = result + c_array[i];
        printf(result);
        for (j = 2; j >= 0; j = j - 1)
        {
            result = result + c_matrix[i][j];
            printf(result);
        }
    }
    printf("char sum:");
    printf(result);
    result = i_matrix[2][2];
    printf("i_matric[2][2]=", result);
    result_c = c_matrix[2][2];
    printf(result_c);
    switch (i_matrix[1][1])
    {
break;
    case 0:
        result = 0;
break;
    case 1:
        result = 1;
break;
    case 2:
        result = 2;
break;
    case 3:
        result = 3;
break;
    case 12:
        result = 4;
break;
    case 100:
        { }
break;
    default:
        result = 5;
    }
    printf("switch to case", result);
    if (result <= 4)
    {
        printf("if success!");
    }
    if (result >4)
    {
        result = 0;
    }
    else
    {
        result = 1;
    }
    printf("else success: ", result);
    result = i_array[0];
    i_array[0] = i_matrix[0][0];
    i_matrix[0][0] = result;
    result = i_1 + i_2 + i_3 + i_4;
    for (i = 0; i != 3; i = i + 1)
    {
        result = result + i_array[i];
        j = 2;
        while (j >= 0)
        {
            result = result + i_matrix[i][j];
            j = j - 1;
        }
    }
    printf("int sum:");
    printf(result);
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                