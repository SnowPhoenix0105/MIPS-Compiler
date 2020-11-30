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

void increase()
{
    a = a + 1;
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

int bar(int i_1, int i_2, int i_3, int i_4, int i_5, int i_6, int i_7, int i_8, int i_9, int i_10,
        int i_11, int i_12, int i_13, int i_14, int i_15, int i_16)
{
    return(i_1 + i_2 + i_3 + i_4 + i_5 + i_6 + i_7 + i_8 + i_9 + i_10 +
        i_11 + i_12 + i_13 + i_14 + i_15 + i_16);
}


int foobar(int i_1, int i_2, int i_3, int i_4, int i_5, int i_6, int i_7, int i_8, int i_9, int i_10,
        int i_11, int i_12, int i_13, int i_14, int i_15, int i_16, 
        int i_21, int i_22, int i_23, int i_24, int i_25, int i_26, int i_27, int i_28, int i_29, int i_30,
        int i_31, int i_32, int i_33, int i_34, int i_35, int i_36)
{
    int ret;
    ret = i_1 + i_2 + i_3 + i_4 + i_5 + i_6 + i_7 + i_8 + i_9 + i_10 +
        i_11 + i_12 + i_13 + i_14 + i_15 + i_16;
    if (i_1 > i_2)
    {
        return(ret);
    }
    ret = i_21 + i_22 + i_23 + i_24 + i_25 + i_26 + i_27 + i_28 + i_29 + i_30 +
        i_31 + i_32 + i_33 + i_34 + i_35 + i_36;
    return(ret);
}
void main()
{
    int i;
    for (i = 0; i < 7; i = i + 1)
    {
        printf(randomi());
        printf(get_increase());
        printf(
            foo(
                get_increase(), 
                randomi(), 
                randomi(), 
                randomi(), 
                randomi(), 
                randomi()
            )
        );
        printf(
            foobar(1,2,3,4,5,6,7,8,9,10,
                 11,12,13,14,15,16,21,22,
                 23,24,25,26,27,28,29,30,
                 31,32,33,
                    34,35,36)
        );
    }
}