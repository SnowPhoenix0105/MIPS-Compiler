const int const_int = + 1;
const char const_char = 'a', const_num = '0', const_add = '+', const_div = '/';
int var, array[100], matrix[10][10];
char c = '0';
int func1(int val1, int val2) {
    const int const_val = -10;
    int val = 10;
    val = val1 * val + val2;
    return (1);
}
char func2(char c) {
    char str[2] = {'0', '1'};
    return (str[1]);
}
void func3() {
    char strs[2][2] = {{'0', '1'}, {'2', '3'}};
    return;
}
void func4() {

}
void main() {
    int a, b, c, i, j;
    char c1, c2, c3;
    a = 1;
    b = -10;
    while(a < 10) {
        array[a] = a;
        a = a + 1;
    }
    for(i = 1; i <= 9; i = i + 1) {
        for (j = 9; j >= 1; j = j - 1) {
            array[i] = i * 9 + j;
            matrix[i][j] = i + j;
        }
    }
    if (a == 1) {
        a = +10;
    }
    if (a != 1) {
        a = 11;
    }
    if (a > 10) {
        a = 10;
    }
    if (b - 1 < 1) {
        b = 100 + b;
    } else {
        b = 99 + b;
    }
    c = func1(a, b);
    c1 = func2('a');
    func3();
    scanf(c2);
    printf("18373326");
    printf("Hello World!");
    switch (a) {
        case 1:
            a = a * 1;
        default:;
    }
    switch (b) {
        case 1:
            b = b / 1;
        case 2:
            b = b + 1;
        default:;
    }
    printf("1");
    printf("2");
    printf("3");
    printf("4");
    printf("5");
    printf("6");
    printf("7");
    printf("8");
}
