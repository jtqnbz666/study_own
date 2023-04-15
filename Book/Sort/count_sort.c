#include <stdio.h>
#include <stdlib.h>
#define NUM 10

void count_sort(int *arr, int MaxNum) {
    int * pattern = (int *)calloc(sizeof(int), MaxNum + 1);  //calloc分配空间帮你初始化为0
    for(int i = 0; i < NUM; i++) { 
        pattern[arr[i]] ++;
    }
    int k = 0;
    for(int i = 0; i < MaxNum + 1; i++) {
        if(pattern[i] != 0) {
            arr[k++] = i;
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
    int arr[NUM] = {1, 9, 20, 3, 58, 52, 99, 131, 22, 79};
    print_test(arr,1);
    count_sort(arr,131);
    print_test(arr,0);
    return 0;
}