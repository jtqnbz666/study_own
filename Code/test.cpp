#include   <bits/stdc++.h>
using namespace std;
void quick_sort(int *arr, int left, int right) {
    if(left >= right) return;
	int l = left, r = right;
    int mid = arr[(l + r) / 2];
    while(l <= r) {
        while(arr[l] < mid) l++;
        while(arr[r] > mid) r--;
        if(l <= r) {
            swap(arr[l], arr[r]);
            l ++, r --;
        }
    }
    quick_sort(arr, left, r);
    quick_sort(arr, l , right);
}
int main() {
	int arr[10] = {3, 2, 1, 7, 9, 5, 4, 6, 0, 8};
	quick_sort(arr, 0, 9); 
	for(auto v : arr) {
	 	cout << v << " ";
	}  
}
