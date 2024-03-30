普遍时间复杂度N * logN, 极端情况，每次都取到最大或者最小基准值，退化为冒泡(N^2), 有序的数组用插入排序是最快的O(n)

快排的优化：1.单边递归(减少了一半的递归，本来是把左边部分递归进一个函数的，但单边递归直接在当前层处理，减少一半递归)2.三点取中法(三个数中取第二大的数，避免取到最大或者最小，影响效率可能退化为O(n^2))，3.partition操作，一次或者多次递归就能得到想要的结果，比如颜色分类，奇数排偶数前

普通版快排

~~~c++
#include   <bits/stdc++.h>
using namespace std;
void quick_sort(int *arr, int left, int right) {
    if(left >= right) reutrn 0;
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
~~~

模仿stl当分区小于16时采用插入排序， 但没有考虑堆排序，并且使用了单边递归+三点取中快排

~~~c++
class Solution {
public:
    int getMid(int a, int b, int c) {
        if(a > b) swap(a, b);
        if(a > c) swap(a, c);
        if(b > c) swap(b, c);
        return b;
    }
    void quick_sort(vector<int>& nums, int begin, int end) {
        while(end - begin > 16) {
            int l = begin, r = end;
            int mid = getMid(nums[begin], nums[end], nums[(begin + end) / 2]);
            while(l <= r) {
                while(nums[l] < mid) l ++;
                while(nums[r] > mid) r --;
                if(l <= r) {
                    swap(nums[l], nums[r]);
                    l ++,  r --;
                }
            }
            quick_sort(nums, l, end);
            end = r;
        }
    }
    void insert_sort(vector<int>& nums, int begin, int end) {
        int Min = nums[0];
        int idx = 0;
        for(int i = 0; i < nums.size(); i ++) {
            if(nums[i] < Min) {
                Min = nums[i];
                idx = i;
            }
        }
        swap(nums[0], nums[idx]);
        for(int i = 2; i < nums.size(); i ++) {
            int j = i;
            int tmp = nums[j];
            while(tmp < nums[j - 1]) {
                nums[j] = nums[j - 1];
                j --;
            }
            nums[j] = tmp;
            //把上边6行换成这样也行
            /*while(nums[j] < nums[j - 1]) {
                swap(nums[j], nums[j - 1]);
                j --;
            }*/
        }
    }
    vector<int> sortArray(vector<int>& nums) {
        quick_sort(nums, 0, nums.size() - 1);
        insert_sort(nums, 0, nums.size() - 1);
        return nums;
    }
};
~~~

1.lc912排序数组，单纯的快排会超时，需要优化

2.剑指offer21 调整数组顺序使奇数位于偶数前面， partition一次遍历

3.排序链表， 使用double值记录基准值

4.lc75颜色分类，两次partition操作, 或者一次3路partition操作

两次partition更好理解

~~~c++
class Solution {
public:
    void partation(vector<int>& nums, int idx) {
        int l = 0, r = nums.size() - 1;
        while(l < r) {
            while(l <= r && nums[l] <= idx) {
                l ++;
            }
            while(l <= r && nums[r] > idx) {
                r --;
            }
            if(l <= r) {
                swap(nums[l], nums[r]);
                l ++; r --;
            }
        }
    }
    void sortColors(vector<int>& nums) {
        partation(nums, 0);
        partation(nums, 1);
    }
};
~~~

一次三路partition，其实就是多一个指针用来记录

~~~c++
class solution {
public:
    void partition(vector<int>& nums, int idx) {
        int index = 0, l = 0, r = nums.size() - 1;
        while(index <= r) {
            if(nums[index] == 0) {
                swap(nums[index], nums[l]);
                l ++, index ++;
            } else if (nums[index] == 2) {
                swap(nums[index], num[r]);
                r --;
            } else if (nums[index] == 1) {
                index ++;
            }
        }
    }
    void sortColor(vector<int>& nums) {
        partition(nums, 1);
        return;
    }
};
~~~

