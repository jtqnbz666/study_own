#include <stdio.h>
#include <stdlib.h>
void radix_sort( int *arr, int len, int MaxNum ) {
    int * pattern = (int *)malloc(sizeof(int)*(MaxNum+1));  //因为没有用下标0，所以多开一个空间。
    for( int i = 0 ; i < MaxNum + 1; i ++ ) {
        pattern[i] = -1;
    }
    for( int i = 0; i < len ;i ++ ) {
        pattern[arr[i]] = arr[i];
    }
    int k = 0;
    for( int i = 0 ; i < MaxNum + 1; i ++ ) {
        if( pattern[i] != -1 ) {
        arr[k++] = pattern[i];
        }
    }
}
void print_test(int * arr, int isfirst) {  
    if(isfirst) {
        printf("排序前：\n");
    } else {
        printf("排序后：\n");
    }
    for( int i = 0 ; i < 10; i ++ ) {
        printf("%d\t", arr[i]);        
    }
    printf("\n");
}
int main() {
    int arr[10] = {1, 9, 20, 3, 58, 52, 99, 131, 22, 79};
    print_test(arr,1);
    radix_sort(arr,10, 131);
    print_test(arr,0);
    return 0;
}