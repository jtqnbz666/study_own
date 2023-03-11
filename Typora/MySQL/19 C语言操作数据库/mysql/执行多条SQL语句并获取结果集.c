#include<stdio.h>
#include<mysql.h>

int main()
{
	//��ʼ��mysql
	MYSQL* mysql = mysql_init(NULL);

	//д��һ�����host�������˺ܾòŵ���ʧ��
	//�������ӳ�ʱ��ʱ��
	int timeout = 3;
	if (0 != mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout))
	{
		printf("mysql_options failed %s\n", mysql_error(mysql));
	}
	//���öϿ�����
	bool reconnect = true;
	if (0 != mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect))
	{
		printf("mysql_options failed %s\n", mysql_error(mysql));
	}
	//���ñ���
	mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES GBK");

	//����mysql������
	if (!mysql_real_connect(mysql, "localhost", "root", "123456", "test", 3306, NULL, CLIENT_MULTI_STATEMENTS))
	{
		printf("connect failed��%s\n", mysql_error(mysql));
		return -1;
	}
	else
	{
		printf("connect successful!\n");
	}

	//1��ִ��SQL���
	const char* sql = "select * from emp;select * from dept;"
		"insert into emp(empno,ename,sal) values(525,'��ʯ',20000)";
	//mysql_real_query(mysql, sql, strlen(sql));
	if (0 != mysql_query(mysql, sql))
	{
		printf("mysql_query failed:%s\n", mysql_error(mysql));
	}

	//2����ȡ�����
	do
	{
		MYSQL_RES* result = mysql_store_result(mysql);	//��ѽ�������浽����
		//MYSQL_RES* result = mysql_use_result(mysql);		//����ѽ������������������һ����¼һ����¼�Ĵӷ�������ȡ
		if (!result)
		{		
			//û�н�������������ֶΣ��Ͳ����˴���
			if (mysql_field_count(mysql) > 0)
			{
				printf("mysql_store_result failed %s\n", mysql_error(mysql));
			}
			//insert update delete û�н�������е�ֻ��Ӱ�������
			else
			{
				printf("affected row: %llu\n", mysql_affected_rows(mysql));
			}
		}
		else
		{
			//��ȡ�ֶ�����
			unsigned int fieldCnt = 0;
			fieldCnt = mysql_num_fields(result);

			//��ȡÿ���ֶΣ������
			MYSQL_FIELD* field = NULL;
			//2,�Ȱ����е��ֶζ�ץȡ������Ȼ��ȥ����
			field = mysql_fetch_fields(result);
			if (field)
			{
				for (int i = 0; i < fieldCnt; i++)
				{
					printf("%-*s ", field[i].max_length + 1, field[i].name);
				}
			}
			putchar('\n');

			//��ȡÿ����¼
			MYSQL_ROW row = NULL;
			while (row = mysql_fetch_row(result))
			{
				//����������¼
				for (int i = 0; i < fieldCnt; i++)
				{
					printf("%-*s ", field[i].max_length + 1, row[i] ? row[i] : "NULL");
				}
				printf("\n");
			}


			//�����ͷŽ����
			mysql_free_result(result);
		}
	} while (mysql_next_result(mysql) == 0);	//����н������������ȡ�����
	//��ȡ��������֮�󣬿���һ����û�д���
	if (mysql_errno(mysql) != 0)
	{
		printf("error:%s\n", mysql_error(mysql));
	}

	//�ر�mysql
	mysql_close(mysql);
	return 0;
}