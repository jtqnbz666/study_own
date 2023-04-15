#include <stdio.h>
#include <stdlib.h>
int size = 0;
void heap_sort(int * heap, int val) {
    int hole = size ++ ;
    int parent = 0;
    for(; hole > 0 ; hole = parent) {
        parent = (hole - 1) / 2;
        if(heap[parent] < val) {
            break;
        }
        heap[hole] = heap[parent];
    }
    heap[hole] = val;
}
void pop(int * heap) {
    int tmp = heap[0];
    heap[0] = heap[--size];
    heap[size] = tmp;
    tmp = heap[0];  //这步不能少 
    //调整
    int parent = 0;
    int child = 0; 
    for(; (parent * 2 + 1) <= (size - 1); parent = child) {
        child = parent * 2 + 1;
        if((child < size - 1 ) && heap[child + 1] < heap[child]) {
            child += 1;
        }
        if(heap[child] < tmp) {
            heap[parent] = heap[child];
        } else {
            break;
        }
    }
    heap[parent] = tmp;
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
    int * heap = (int *)malloc(sizeof(int) * 10);
    for(int i = 0 ; i < 10; i ++ ) {
        heap[i] = -1;
    }
    for( int i = 0; i < 10; i ++ ) {
        heap_sort(heap, arr[i]);        //建堆
    }
    for(int i = 0; i < 10; i ++ ) {
        pop(heap);                       //依次取出，形成有序数组，不过因为这里用的是小顶堆，所以是降序
    }
    print_test(heap,0);     //打印测试
    return 0;
}