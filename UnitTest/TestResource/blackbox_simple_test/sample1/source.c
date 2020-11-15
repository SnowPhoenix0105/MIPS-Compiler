int gvar1;
char garr[5] = {'a', 'b', 'c', 'd', 'e'};
int gvar2 = 18231045;

void main() {
    char larr1[3][2] = {{'f', 'g'}, {'h', 'i'}, {'j', 'k'}};
    int larr2[2];
    int larr3[4] = {18231045, 10451823, 1881110987, -1};
    int buffer;
    int tmp;
    printf(larr1[1][1]);
    scanf(buffer);
    larr3[2] = buffer;
    tmp = larr3[2];
    printf("your input is\t", tmp);
}