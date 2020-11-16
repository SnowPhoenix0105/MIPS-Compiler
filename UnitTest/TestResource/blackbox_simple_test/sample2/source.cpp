
#include <iostream>

namespace mm {
void __scanf(char& c)
{
	std::cin >> c;
}

void __scanf(int& i)
{
	std::cin >> i;
}

void __printf(const char* msg)
{
	std::cout << msg << std::endl;
}
void __printf(const char* msg, char c)
{
	std::cout << msg << c << std::endl;
}
void __printf(const char* msg, int c)
{
	std::cout << msg << c << std::endl;
}
void __printf(char c)
{
	std::cout << c << std::endl;
}
void __printf(int c)
{
	std::cout << c << std::endl;
}


const int b1 = 10, b2 =12;
const char d1 = '1', d2 = '_', d3 = 'a', d4 = '+';
int a1 = 1;
int a2 = 2; 
int a3 = 3; 
int a4 = 4; 
int a5 = 5;
int a12,a13,a14;
char c1 = 'a';
char c2 = 'a';
char c3 = 'a';
char c4 = 'a';
char c5 = 'X';
char c6 = '4';
void main(){
	const int b3 = 13, b1 = 14;
	const char d2 = '9', d5 = '/';
	const int xxx = -1;
	int a3, a4;
	char c7, c4, c8;
	char c9 = 'e';
	char c10 = '1';
	int a6 = 6; 
	int a7 = 7; 
	int a8 = 8; 
	int a9 = 9; 
	int a10 = 10; 
	int a11 = 11;

	__scanf(a3);
	__printf(a3);
	a3 = a3 * ((-'3') * (3));
	__printf(a3);
	__scanf(a6);
	a7 = 18;
	__scanf(a7);
	__scanf(c1);
	__scanf(c7);
	a3 = -a2;
	__printf(a3);
	a3 = a3*a3;
	__printf(a3);
	a3 = a3;
	__printf(a3);
	a3 = -d3-(-d3-(-d3))-d4*(b2/2)+b3*(a1+b1);
	__printf(a3);
	__printf(3+4+5+6-7-8-9*4*4/3/2*(2+3));
	c8 = c10;
	c4 = c1;
	a1 = 12;
	a2 = 13;
	a3 = 14;
	a4 = 15;
	a5 = 16;
	a6 = 17;
	a8 = 19;
	a9 = 20 + a9;
	a10 = a1 + 21;
	a11 = a1 +a10;
	__printf('a');
	__printf(a1);
	__printf(a1+'a');
	__printf(-5*(-5));
	__printf("a4",a4);
	__printf(3);
	__printf("18231217",18231217);
	__printf(d1);
	__printf(d2);
	__printf(-d1-d2*a3+b1*(-3)+(3-4+2-'4'+60)*4/1/1/1);
	__printf(a1);
	__printf(a2);
	__printf(a3);
	__printf(a8);
	__printf(a9);
	__printf(a10);
	__printf(a11);
	__printf("134214321432143214321");
	__printf(-a11 * (a3- a1*(-a2+a5)/(a2+a1+2)-a1*(-a1)+3*5+a6)+a3*(-a4+a2)+a7);
	a1 = -('a'*(a4+(d5+c9*a6)-3*(a9*'2'-10*(b2*d1)/4)*(-1*45-(a1+a2)-(23*3-(c1+a4)*(c3+a9)/100)*(+342-a6*(a9-b1))/4))+42)-0+4*a1;
	__printf(a1);
	__printf(a4);
	__printf(a8);
	__printf(c1);
	a2 = a1 + a1;
	a3 = +-36000/(a10+c3)/(+-4*(a6+(b3*(b2*(a2-4))+a4*(a6+24)/'1'*10-14+42*a2-b1*a9+c1+c2)+(a1-a3)*(a3+(a5-a2*(b2+d1)/12)+42)+0)+24*b2+(a3*a5+c1)*2)+a3*(a4-b2+a5)/(2+a2);
	__printf(a3);
	__printf("yes",a2);
	__printf(a4);
	__printf(xxx);
}

}

int main(int argc, char const *argv[])
{
	mm::main();
	return 0;
}
