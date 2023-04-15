#include <stdio.h>
#include <stdlib.h>
#define NUM 10 //需要排序的数字个数
#define RANGE 10e3 //需要排序的数字最大的数字的次幂
void bucket_sort(int * arr, int len , int range) {
    int idx = 0;
    for( int i = 1; i < range ; i *= 10 ) {
       
        int * pattern = (int *)malloc (sizeof(int)*len*10);
        for( int i = 0;i < len*10 ; i ++ ) {
            pattern[i] = -1;
        }
        for( int j = 0; j < len ; j ++ ) {
           idx = arr[j]/i%10;
           *(pattern+idx*10+j) = arr[j];
        }
        int k = 0;
        for( int h = 0; h < len*10 ; h ++ ) {
           if(pattern[h] != -1) {
                arr[k++] = pattern[h]; 
           }
        }
    }
}
void print_test(int * arr, int isfirst) {  
    if(isfirst) {
        printf("排序前：\n");
    } else {
        printf("排序后：\n");
    }
    for( int i = 0 ; i < NUM; i ++ ) {
        printf("%d\t", arr[i]);        
    }
    printf("\n");
}
int main() {
    int arr[NUM] = {1, 9, 20, 3, 58, 52, 99, 131, 22, 79};
    print_test(arr,1);
    bucket_sort(arr, NUM, RANGE);
    print_test(arr,0);
    return 0;
}