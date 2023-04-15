
#include <bits/stdc++.h>
using namespace std;


void MergeTwoIntervals(int *arr,int l1,int r1,int l2,int r2){
    int *temp=(int *)malloc(sizeof(int)*(r2-l1+1));
    int i=l1,j=l2,k=0;
    while(i<=r1||j<=r2){
        if((j>r2)||(i<=r1&&arr[i]<=arr[j])){
            temp[k++]=arr[i++];
        }else{
            temp[k++]=arr[j++];
        }
    }
    for(int i=l1;i<=r2;i++){
        arr[i]=temp[i-l1];
    }
    free(temp);
}
void MergeSort(int* arr,int l,int r){
    if(l >= r){
        return ;
    }
    int mid = (l + r)/2;
    MergeSort(arr,l,mid);
    MergeSort(arr,mid+1,r);
    MergeTwoIntervals(arr,l,mid,mid+1,r);
}
int main(){

     int arr[14]={2,-1,4,1,7,6,3,6,9,8,12,7,9,7};
    MergeSort(arr,0,13);
    for(int i=0;i<14;i++){
        cout<<arr[i]<<" ";
    }
return 0;
}