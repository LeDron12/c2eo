//clang -Xclang -ast-dump -fsyntax-only main.cpp
//#include <stdio.h>
int x;
int main() {
    int a = 3;
    int b = 2;
    int c = a + b;
    a;
    //printf("%d\n", a);
    b;
    //printf("%d\n", b);
    c;
    //printf("%d\n", c);
}
