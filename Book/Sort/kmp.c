#include <iostream>
#include <string>
#include<cstring>
using namespace std;

int GetNextJ(const char* pattern,int *next,char ch,int j){
    while(j!=-1&&pattern[j+1]!=ch){
        j=next[j];
    }
    if(pattern[j+1]==ch){
        j+=1;
    }
    return j;
}
void GetNext(int * next,const char* pattern){
    for(int j=-1,i=1;pattern[i];i++){
        while(j!=-1&&pattern[j+1]!=pattern[i]){
            j=next[j];
        }
        if(pattern[j+1]==pattern[i])j+=1;
             next[i]=j;
    }
}
int kmp(const  char* pattern, const char * text){
    int nLength=strlen(pattern);
    int *next=(int *)malloc(sizeof(int)*nLength);
    GetNext(next,pattern);
    
    for(int j=-1,i=0;text[i];i++){
        j=GetNextJ(pattern,next,text[i],j);
        if(pattern[j+1]=='\0')return i-j;
    }
    return -1;
}

int main(){
    char s1[100],s2[100];
    while(cin>>s1>>s2){
        cout<<s2<<"  in"<<s1<<"  result  "
        <<kmp(s1,s2);
    }
    return 0;
}