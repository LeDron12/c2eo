#include <stdio.h>
int
test()
{
	int arr[2];
	int *p;
	
	p = &arr[1];
	p -= 1;
	*p = 123;
	
	if(arr[0] != 123)
		return 1;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}