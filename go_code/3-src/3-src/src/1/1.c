#include <stdio.h>  
#include <string.h>
char *fun(char *p1, char *p2)
{
    int i = 0;
    i = strcmp(p1, p2);
    if (0 == i)
    {
        return (p1);
    }
    else
    {
        return (p2);
    }
}
int main()
{
    char *(*pf)(char *p1, char *p2);
    pf = &fun;
    (*pf)("aa", "bb");
    return (0);
}