#include <stdlib.h>
int main() {
const int a = 10;
int *b = (int*)&a;
*b = 111;
printf("%d,%d",a, *b);
return 0;
}
