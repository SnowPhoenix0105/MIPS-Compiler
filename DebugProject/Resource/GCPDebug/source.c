int a = 0;
int arr[8] = { 18231045, 2314122, 2312323, 9087, 2, 123123, 232897, 678};
int idx1=0;
int idx2=4;


int randomi()
{
    int ret;
    ret = arr[idx1] - arr[idx2];
    arr[idx1] = ret;
    idx1 = idx1 + 1;
    if (idx1 >= 8)
    {
        idx1 = idx1 - 8;
    }
    idx2 = idx2 + 1;
    if (idx2 >= 8)
    {
        idx2 = idx2 - 8;
    }
    return(ret);
}

int get_increase()
{
    a = a + 1;
    return(a);
}


int foo(int i, int j, int k, int l, int m, int n)
{
    if (i > j)
    {
        l = m + randomi();
    }
    else
    {
        l = m * m / 123 + (312 + (i + j / k - (m + n) * 4) / 324) + k *n;
    }
    if (j > k)
    {
        m = n + randomi();
    }
    else
    {
        for (i = 0; i < 4; i = i + 1)
        {
            m = get_increase() * n;
        }
    }
    
    return(l + m);
}

void main()
{
    int i;
    for (i = 0; i < 7; i = i + 1)
    {
        printf(randomi());
        printf(get_increase());
    }
}