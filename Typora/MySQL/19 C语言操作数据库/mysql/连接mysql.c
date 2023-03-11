#include<stdio.h>
#include<mysql.h>

int main()
{
	//��ʼ��mysql
	MYSQL *mysql = mysql_init(NULL);

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

	//���������ʹ�õĹ����У���������Ͽ������ӣ�1������ 2��ֱ�ӷ�����������
	while (true)
	{
		if (0 != mysql_ping(mysql))
		{
			printf("mysql_ping ��%s\n", mysql_error(mysql));
		}
		else
		{
			printf("mysql_ping successful!\n");
		}
		Sleep(1000);
	}

	//�ر�mysql
	mysql_close(mysql);
	return 0;
}