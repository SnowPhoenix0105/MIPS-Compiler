int arry[2][2] = {{0,0},{0,0}};
int test_forsub(){
	int n;
	int a = 0;
	for(n=3;n>0;n=n-1){
		a = a+n;
	}
	return(a);
}
int test_foradd(){
	int n;
	int a = 0;
	for(n=0;n<=3;n=n+1){
		a = a+n;
	}
	return(a);
}
int cal(int a){
	return(a*a);
}
int add(int a, int b)
{
    return (a + b);
}
void test_function_call()
{
    int n;
    n = add(
            add(1, cal(2)),
            cal(add(1, 2))
        );
    printf(n);
}
void main(){
	int a;
	int b;
	int c;
	int ch0;
	int ch1;
	;
	printf("18182651");
	a = test_forsub();
	printf(a);
	c = test_foradd();
	printf(c);
	printf(arry[1][1]);
	printf("empty_sentence");
	{	
	}
	scanf(b);
	printf(b);
	b = cal(b);
	printf(b);
	test_function_call();
	scanf(ch0);
	printf(ch0);
	scanf(ch1);
	if(ch0 < ch1)
		printf("small");
		else
			printf("not equal");
}