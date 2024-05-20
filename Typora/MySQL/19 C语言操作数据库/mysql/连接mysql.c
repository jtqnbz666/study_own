#include<stdio.h>
#include<mysql.h>

int main()
{
	//初始化mysql
	MYSQL *mysql = mysql_init(NULL);

	//写了一个错的host，连接了很久才弹出失败
	//设置连接超时的时间
	int timeout = 3;
	if (0 != mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout))
	{
		printf("mysql_options failed %s\n", mysql_error(mysql));
	}
	//设置断开重连
	bool reconnect = true;
	if (0 != mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect))
	{
		printf("mysql_options failed %s\n", mysql_error(mysql));
	}


	//连接mysql服务器
	if (!mysql_real_connect(mysql, "localhost", "root", "123456", "test", 3306, NULL, 0))
	{
		printf("connect failed：%s\n", mysql_error(mysql));
		return -1;
	}
	else
	{
		printf("connect successful!\n");
	}

	//如果我们在使用的过程中，与服务器断开了连接，1，重连 2，直接放弃，不重连
	while (true)
	{
		if (0 != mysql_ping(mysql))
		{
			printf("mysql_ping ：%s\n", mysql_error(mysql));
		}
		else
		{
			printf("mysql_ping successful!\n");
		}
		Sleep(1000);
	}

	//关闭mysql
	mysql_close(mysql);
	return 0;
}