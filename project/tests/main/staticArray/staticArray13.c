#include "stdio.h"

long long a[5][7];
int main() {
    a[1][3] = 10;
    a[3][1] = 20;
      printf("%lld\n", a[1][3]);
      printf("%lld\n", a[3][1]);
    return 0;
}
