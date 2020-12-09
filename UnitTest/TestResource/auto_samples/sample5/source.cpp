
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                
int value=0;
int i,j;
void main() {
	for (i=0;i<5;i=i+1) {
		for (j=0;j<5;j=j+1){
			value = value + 1;
		}
	}
	printf(value);
}

                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                