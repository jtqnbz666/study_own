#include<stdio.h>
#include<mysql.h>

int main()
{
	//初始化mysql
	MYSQL* mysql = mysql_init(NULL);

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
	//设置编码
	mysql_options(mysql, MYSQL_INIT_COMMAND,"SET NAMES GBK");

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

	//1，执行SQL语句
	const char* sql = "select * from emp";
	//mysql_real_query(mysql, sql, strlen(sql));
	if (0 != mysql_query(mysql, sql))
	{
		printf("mysql_query failed:%s\n", mysql_error(mysql));
	}

	//2，获取结果集
	MYSQL_RES* result = mysql_store_result(mysql);	//会把结果集保存到本机
	//MYSQL_RES* result = mysql_use_result(mysql);		//不会把结果集保存起来，而是一条记录一条记录的从服务器获取
	if (!result)
	{
		printf("mysql_store_result failed %s\n", mysql_error(mysql));
	}
	else
	{
		//获取字段数量
		unsigned int fieldCnt = mysql_field_count(mysql);
		//printf("fieldCnt:%lu\n", fieldCnt);
		//获取每条记录
		MYSQL_ROW row = NULL;
		while (row = mysql_fetch_row(result))
		{
			//遍历该条记录
			for (int i = 0; i < fieldCnt; i++)
			{
				printf("%-10s ", row[i] ? row[i] : "NULL");
			}
			printf("\n");
		}


		//必须释放结果集
		mysql_free_result(result);
	}

	//关闭mysql
	mysql_close(mysql);
	return 0;
}