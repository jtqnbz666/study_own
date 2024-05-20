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
	if (!mysql_real_connect(mysql, "localhost", "root", "123456", "test", 3306, NULL, 0))
	{
		printf("connect failed��%s\n", mysql_error(mysql));
		return -1;
	}
	else
	{
		printf("connect successful!\n");
	}

	//1��ִ��SQL���
	const char* sql = "select * from emp";
	//mysql_real_query(mysql, sql, strlen(sql));
	if (0 != mysql_query(mysql, sql))
	{
		printf("mysql_query failed:%s\n", mysql_error(mysql));
	}

	//2����ȡ�����
	MYSQL_RES* result = mysql_store_result(mysql);	//��ѽ�������浽����
	//MYSQL_RES* result = mysql_use_result(mysql);		//����ѽ������������������һ����¼һ����¼�Ĵӷ�������ȡ
	if (!result)
	{
		printf("mysql_store_result failed %s\n", mysql_error(mysql));
	}
	else
	{
		//��ȡ�ֶ�����
		unsigned int fieldCnt = 0;
		//fieldCnt = mysql_field_count(mysql);
		fieldCnt = mysql_num_fields(result);
		//printf("fieldCnt:%lu\n", fieldCnt);

		//��ȡ��¼������
		uint64_t rowCnt =  mysql_num_rows(result);
		//printf("%llu\n", rowCnt);

		//��ȡÿ���ֶΣ������
		MYSQL_FIELD *field =NULL;
#define FIELDNO 1
#if FIELDNO == 0
		//1��һ��һ���ֶε�ץȡ
		while (field =  mysql_fetch_field(result))
		{
			printf("%s ", field->name);
		}
#elif FIELDNO == 1	//�Ƽ�ʹ����һ������������������ʱ�򣬷���ʹ��
		//2,�Ȱ����е��ֶζ�ץȡ������Ȼ��ȥ����
		field = mysql_fetch_fields(result);
		if (field)
		{
			for (int i = 0; i < fieldCnt; i++)
			{
				printf("%-*s ",field[i].max_length +1, field[i].name);
			}
		}
#elif FIELDNO == 2
		for (int i = 0; i < fieldCnt; i++)
		{
			printf("%s ", mysql_fetch_field_direct(result, i)->name);
		}
#endif
		putchar('\n');


		//��ȡÿ����¼
		MYSQL_ROW row = NULL;
		while (row = mysql_fetch_row(result))
		{
			//����������¼
			for (int i = 0; i < fieldCnt; i++)
			{
				printf("%-*s ",field[i].max_length + 1, row[i] ? row[i] : "NULL");
			}
			printf("\n");
		}


		//�����ͷŽ����
		mysql_free_result(result);
	}

	//�ر�mysql
	mysql_close(mysql);
	return 0;
}