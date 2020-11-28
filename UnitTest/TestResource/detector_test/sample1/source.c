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

int get_one()
{
    a = a + 1;
    return(a);
}

void main()
{
    int i;
    for (i = 0; i < 7; i = i + 1)
    {
        print(randomi());
        print(get_one());
    }
}