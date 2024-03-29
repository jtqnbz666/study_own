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
	mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES GBK");

	//连接mysql服务器
	if (!mysql_real_connect(mysql, "localhost", "root", "123456", "test", 3306, NULL, CLIENT_MULTI_STATEMENTS))
	{
		printf("connect failed：%s\n", mysql_error(mysql));
		return -1;
	}
	else
	{
		printf("connect successful!\n");
	}

	//1，执行SQL语句
	const char* sql = "select * from emp;select * from dept;"
		"insert into emp(empno,ename,sal) values(525,'顽石',20000)";
	//mysql_real_query(mysql, sql, strlen(sql));
	if (0 != mysql_query(mysql, sql))
	{
		printf("mysql_query failed:%s\n", mysql_error(mysql));
	}

	//2，获取结果集
	do
	{
		MYSQL_RES* result = mysql_store_result(mysql);	//会把结果集保存到本机
		//MYSQL_RES* result = mysql_use_result(mysql);		//不会把结果集保存起来，而是一条记录一条记录的从服务器获取
		if (!result)
		{		
			//没有结果集，但是有字段，就产生了错误
			if (mysql_field_count(mysql) > 0)
			{
				printf("mysql_store_result failed %s\n", mysql_error(mysql));
			}
			//insert update delete 没有结果集，有的只是影响的行数
			else
			{
				printf("affected row: %llu\n", mysql_affected_rows(mysql));
			}
		}
		else
		{
			//获取字段数量
			unsigned int fieldCnt = 0;
			fieldCnt = mysql_num_fields(result);

			//获取每个字段，并输出
			MYSQL_FIELD* field = NULL;
			//2,先把所有的字段都抓取出来，然后去遍历
			field = mysql_fetch_fields(result);
			if (field)
			{
				for (int i = 0; i < fieldCnt; i++)
				{
					printf("%-*s ", field[i].max_length + 1, field[i].name);
				}
			}
			putchar('\n');

			//获取每条记录
			MYSQL_ROW row = NULL;
			while (row = mysql_fetch_row(result))
			{
				//遍历该条记录
				for (int i = 0; i < fieldCnt; i++)
				{
					printf("%-*s ", field[i].max_length + 1, row[i] ? row[i] : "NULL");
				}
				printf("\n");
			}


			//必须释放结果集
			mysql_free_result(result);
		}
	} while (mysql_next_result(mysql) == 0);	//如果有结果集，继续获取结果集
	//获取结果集完成之后，看下一下有没有错误
	if (mysql_errno(mysql) != 0)
	{
		printf("error:%s\n", mysql_error(mysql));
	}

	//关闭mysql
	mysql_close(mysql);
	return 0;
}