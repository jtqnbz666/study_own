~~~c++
void GetNext(const char* pattern, int* next) {
    next[0] = -1;
    for(int i = 0, j = -1; pattern[i]; i ++) {
        while(j != -1 && pattern[j + 1] != pattern[i]) j = next[j];
        if(pattern[j + 1] == pattern[i]) j += 1;
        next[i] = j;
    }
}
int GetNextJ(char ch, const char* pattern, int* next, int j) {
    while(j != -1 && pattern[j + 1] != ch) j = next[j];
    if(pattern[j + 1] == ch) j += 1;
    return j;
}
int kmp(const char* text, const char* pattern) {
	int n = strlen(pattern);
    int *next = (int*)malloc(sizeof(int) * n);
    GetNext(pattern, next);
    for(int i = 0, j = -1; text[i]; i ++) {
        j = GetNextJ(text[i], pattern, next, j);
        if(pattern[j + 1] == '\0') return i - j;
    }
    return -1;
}
~~~



