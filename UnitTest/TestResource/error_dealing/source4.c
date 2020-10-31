

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
    bigger(324,  c);
    print_all(i, c, i);
    print_all(i);
    single_char(c);
}