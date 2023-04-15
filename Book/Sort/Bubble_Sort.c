#include <stdio.h>
#define NUM 10
#if 0
void Bubble_sort(int * arr) {   //普通版本
    
    for( int i = 0; i < NUM - 1; i ++ ) {
        for( int j = 0; j < NUM - 1 - i; j ++ ) {
            if(arr[j + 1] < arr[j]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}
# else 
void Bubble_sort(int * arr) {  //优化版。
    int lastExchangeIdx = 0;  //记录最后一次交换的位置
    int unsortBorder = NUM - 1; //记录无序数组的边缘位置

    for( int i = 0; i < NUM - 1; i ++ ) {
        bool IsSort = true; //判断次数循环是否一直处于有序状态，若是说明不用继续了，直接退出
        for  ( int j = 0; j < unsortBorder; j ++ ) {
            if(arr[j + 1] < arr[j]) {
                IsSort = false;
                int tmp = arr[j + 1] ;
                arr[j + 1] = arr[j]; 
                arr[j] = tmp;
                lastExchangeIdx = j;
            }
        }
        unsortBorder = lastExchangeIdx;
        if(IsSort) {  //如果一直都没有调整过位置，说明已经有序了。
            break;  
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
    Bubble_sort(arr);
    print_test(arr,0);
    return 0;
}
#endif