// #include <bits/stdc++.h>
#include <iostream>
using namespace std;

void shell_sort(int *arr, int length){
    int gpa=0,i=0,j=0;
    for(gpa=length/2;gpa>=1;gpa/=2){
        for(i=gpa;i<length;i++){
            int temp=arr[i];
            // for(j=i-gpa;j>=0&&temp<arr[j];j=j-gpa){
            //     arr[j+gpa]=arr[j];
            // }
            // arr[j+gpa]=temp;

            for(j = i; j > 0 && temp < arr[j - gpa]; j-=gpa) {
                arr[j] = arr[j - gpa];
            }
            arr[j] =  temp;
        }
    }
}
int main(){
     int arr[14]={2,-1,4,1,7,6,3,6,9,8,12,7,9,7};
    shell_sort(arr,14);
    for(int i=0;i<14;i++){
        cout<<arr[i]<<" ";
    }

    return 0;

}