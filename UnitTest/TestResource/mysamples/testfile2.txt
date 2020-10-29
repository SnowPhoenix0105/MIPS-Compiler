


int fib(int num)
{
    if (num <= 2)
    {
        return(1);
    }
    return(fib(num - 1) + fib(num - 2));
}

int fib2(int num)
{
    if (num <= 2)
    {
        return(1);
    }
    return(fib(num - 1) + fib2(num - 2));
}

int fib3(int num)
{
    if (num == 2)
    {
        return(1);
    }
    if (num <= 1)
    {
        return(1);
    }
    return(fib(num - 1) + fib2(num - 2));
}



void main()
{
    int result;
    printf("18231045");
    printf("recursive");
    result = fib(10);
    printf("fib(10)=", result);
    result = fib(20);
    printf("fib(20)=", result);
    result = fib2(10);
    printf("fib2(10)=", result);
    result = fib2(20);
    printf("fib2(20)=", result);
    result = fib3(10);
    printf("fib3(10)=", result);
    result = fib3(20);
    printf("fib3(20)=", result);
    result = 1;
    switch (result)
    {
    case 1:
        result = 1;
    default:
        result = 0;
    }
    printf("switch success:", result);
    result = -1;
    printf(result);
}