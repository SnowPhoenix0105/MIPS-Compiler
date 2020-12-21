
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
    case 0:
    {
        printf(0);
    }
    case 4:
    {
        printf(4);
    }
    case 5:
    {
        printf(5);
    }
    case 6:
    {
        printf(6);
    }
    case 7:
    {
        printf(7);
    }
    case 8:
    {
        printf(8);
    }
    default:
        printf("default");
    }
}