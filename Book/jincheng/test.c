#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	for(int i=0;i<10;i++)
	{
		printf("%d:%d\n",getpid(),i);
	}
	printf("被创建进程结束！\n等五秒试试！\n");
	return 0;
}