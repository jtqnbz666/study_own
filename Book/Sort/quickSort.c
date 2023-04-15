#include <iostream>
#include <string>
#include <cstring>
using namespace std;



void quickSort(int *arr,int start,int end){ 
    if(start>=end){
        return ;
    }
    int l=start,r=end;
    int mid=(arr[(l+r)/2]);
    while(l<=r){
        while(arr[l]<mid){//一定不能用l<r, 必须是l<=r
            l++;
        }
        while(arr[r]>mid){
            r--;
        }
        if(l<=r){           //这个if一定不能少，因为l可能会在mid的右边出现
                            //若交换，可能把小的换到后边，大的换到前边。
            swap(arr[l],arr[r]);
            l++;r--;
        }
    }
    quickSort(arr,start,r);
    quickSort(arr,l,end);
}
int main(){
    // int arr[10]={2,-1,4,9,5,6,12,3,1,7};
    int arr[14]={2,-1,4,1,7,6,3,6,9,8,12,7,9,7};
    quickSort(arr,0,13);
    for(int i=0;i<14;i++){
        cout<<arr[i]<<" ";
    }
    

    return 0;
}