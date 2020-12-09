
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                
int a[1000];
int seed[3] = {19971231, 19981013, 1000000007};
int staticvalue = 0;



int set(int pos, int d){
    int x[31];
    int i = 10;
    int v = 0;
    printf(pos);
    printf(d);

    x[0] = 1;
    x[1] = x[0] * 2;
    x[2] = x[1] * 2;
    x[3] = x[2] * 2;
    x[4] = x[3] * 2;
    x[5] = x[4] * 2;
    x[6] = x[5] * 2;
    x[7] = x[6] * 2;
    x[8] = x[7] * 2;
    x[9] = x[8] * 2;
    x[10] = x[9] * 2;
    
    while (i < 30){
        i = i + 1;
        x[i] = x[i - 1] * 2;
    }


    if (pos / 30 >= 1000) return(0);
    if ((a[pos / 30] / (x[pos-pos/30*30]))-(a[pos / 30] / (x[pos-pos/30*30]))/2*2 != d){
        if ((a[pos / 30] / (x[pos-pos/30*30]))-(a[pos / 30] / (x[pos-pos/30*30]))/2*2 == 0)
            if (d == 1)
                v = (x[pos-pos/30*30]);
        
        if ((a[pos / 30] / (x[pos-pos/30*30]))-(a[pos / 30] / (x[pos-pos/30*30]))/2*2 == 1)
            if (d == 0)
                v = v - (x[pos-pos/30*30]);
    }

    a[pos / 30] = a[pos / 30] + v;
    return(0);
}


int rand(){
    staticvalue = staticvalue * seed[0] + seed[1];
    staticvalue = staticvalue - staticvalue / seed[2] * seed[2];
    if (staticvalue < 0) staticvalue = seed[2] + staticvalue;
    return(staticvalue);
}

void main(){
    
    int n;
    int x, y, z;
    int tmp = 0;
    int i = 0;
    int j = 0;
    while(j<1000){
        a[j] = 0;
        j =j + 1;
    }
    scanf(n);
    scanf(staticvalue);
    printf(n);
    printf(staticvalue);
    while (n > 0){
        n = n - 1;
        z = rand();
        printf("[z]    ", z);
        x = z - z / 300000 * 300000;
        printf("[x]    ", x);
        z = rand();
        printf("[z]    ", z);
        y = z - z / 2 * 2;
        printf("[y]    ", y);
        set(x, y);
    }
    while(i<1000){
        tmp = tmp + (a[i] - a[i]/100000 * 100000);
        tmp = tmp - tmp / 100000 * 100000;
        printf(tmp);
        i = i + 1;
    }
    printf(tmp);
    return ;
}
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                