#include <stdio.h>
#define NUM 10
void swap(int * arr, int a, int b) {
    int temp = arr[b];
    arr[b] = arr[a];
    arr[a] = temp;
}
void insert_sort(int * arr) {       //核心代码
    for(int i = 1; i < NUM; i++) {
        for(int k = i; (arr[k] < arr[k - 1]) && k != 0; k--) {
                swap(arr, k, k - 1);
        }
    }

    //我感觉下边这种标准点，上边这种交换太多
    // int k = 0;
    // for(int i = 1; i < NUM; i++) {
    //     int tmp = arr[i];
    //     for(k = i; (tmp < arr[k - 1]) && k != 0; k--) {
    //         arr[k] = arr[k - 1];
    //     }
    //     arr[k] = tmp;
    // }
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
    insert_sort(arr);
    print_test(arr,0);
    return 0;
}