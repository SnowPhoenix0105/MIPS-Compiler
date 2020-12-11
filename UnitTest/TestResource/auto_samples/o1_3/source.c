
const int max = 1073741824;

int a[1000];
int b[1000];
int kernelid[40];

int myscanf()
{
    int n;
    scanf(n);
    return (n);
}

void myprintf(int n)
{
    printf(n);
    return;
}

int checkrange(int num)
{
    while (num > max)
        num = num - max;
    while (num < 0)
        num = num + max;
    return (num);
}

int reduce(int kernelid, int x, int y)
{
    int ret, i;
    if (kernelid == 0)
        return (checkrange(x + y));
    if (kernelid == 1)
    {
        ret = 0;
        i = 1;
        while (i < max)
        {
            if (((x / i) - (x / i) / 2 * 2) == ((y / i) - (y / i) / 2 * 2))
                ret = ret * 2;
            else
                ret = ret * 2 + 1;
            i = i * 2;
        }
        return (ret);
    }
    if (kernelid == 2)
    {
        if (x > y)
            return (x);
        else
            return (y);
    }
    if (kernelid == 3)
    {
        ret = 0;
        i = 1;
        while (i < max)
        {
            if (((x / i) - (x / i) / 2 * 2) == 1)
                ret = ret * 2 + 1;
            else
            {
                if (((y / i) - (y / i) / 2 * 2) == 1)
                {
                    ret = ret * 2 + 1;
                }
                ret = ret * 2;
            }
            i = i * 2;
        }
        return (ret);
    }
    if (kernelid == 4)
    {
        ret = 0;
        i = 1;
        while (i < max)
        {
            if (((x / i) - (x / i) / 2 * 2) == 1)
            {
                if (((y / i) - (y / i) / 2 * 2) == 1)
                    ret = ret * 2 + 1;
                else
                    ret = ret * 2;
            }
            else
                ret = ret * 2;
            i = i * 2;
        }
        return (ret);
    }
    return (0);
}

int getvalue(int n, int m, int x, int y)
{
    int ret;
    if (x < 0)
        return (0);
    if (y < 0)
        return (0);
    if (x >= n)
        return (0);
    if (y >= m)
        return (0);
    ret = a[x * m + y];
    return(ret);
}

int convn(int kernelid, int n, int m, int c)
{
    int i = 0;
    int j = 0;
    int x, y, curr, v;
    int flag1 = 1;
    int flag2 = 1;
    int flag3 = 1;
    int flag4 = 1;
    while (flag1 == 1)
    {
        printf("while_beg flag1");
        j = 0;
        while (flag2 == 1)
        {
            printf("while_beg flag2");
            curr = 0;
            x = i - c / 2;
            while (flag3 == 1)
            {
                printf("while_beg flag3");
                y = j - c / 2;
                while (flag4 == 1)
                {
                    printf("while_beg flag4");
                    printf("n=", n);
                    printf("m=", m);
                    printf("i=", i);
                    printf("j=", j);
                    printf("c=", c);
                    printf("x=", x);
                    printf("y=", y);
                    v = getvalue(n, m, x, y);
                    printf("v=", v);
                    curr = reduce(kernelid, curr, v);
                    printf("curr=", curr);
                    y = y + 1;
                    if (y >= j + c / 2)
                    {
                        flag4 = 0;
                    }
                    printf("while_mid flag4");
                }
                printf("while_end flag4");
                flag4 = 1;
                x = x + 1;
                if (x >= i + c / 2)
                {
                    flag3 = 0;
                }
                printf("while_mid flag3");
            }
            printf("while_end flag3");
            flag3 = 1;
            b[i * m + j] = curr;
            j = j + 1;
            if (j >= m)
            {
                flag2 = 0;
            }
            printf("while_mid flag2");
        }
        printf("while_end flag2");
        flag2 = 1;
        i = i + 1;
        if (i >= n)
        {
            flag1 = 0;
        }
        printf("i=", i);
        printf("flag1=", flag1);
        printf("while_mid flag1");
    }
    printf("while_end flag1");
    return (0);
}

void mymemmove(int n)
{
    int i = 0;
    while (i < n)
    {
        a[i] = b[i];
        i = i + 1;
    }
}

void main()
{
    int c, n, m, task, arrLen, tmp;
    int i = 0;
    c = myscanf();
    n = myscanf();
    m = myscanf();
    arrLen = myscanf();
    while (i < arrLen)
    {
        a[i] = myscanf();
        i = i + 1;
    }
    task = myscanf();
    i = 0;
    while (i < task)
    {
        kernelid[i] = myscanf();
        i = i + 1;
    }

    i = 0;
    while (i < task)
    {
        convn(kernelid[i], n, m, c);
        mymemmove(n * m);
        i = i + 1;
    }

    i = 0;
    while (i < n * m)
    {
        myprintf(a[i]);
        i = i + 1;
    }
    return;
}