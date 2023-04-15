#include <stdio.h>
#define NUM 10
void swap(int * arr, int a, int b) {
    int temp = arr[b];
    arr[b] = arr[a];
    arr[a] = temp;
}
void Select_sort(int * arr) {
    for(int i = 0; i < NUM - 1; i++) {
        int MinIdx = i;
        for(int j = i + 1; j < NUM; j++) {
            if(arr[j] < arr[MinIdx]) {
                MinIdx = j;
            }
        }
        swap(arr, i, MinIdx);
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
    Select_sort(arr);
    print_test(arr,0);
    return 0;
}