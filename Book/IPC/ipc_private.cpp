#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

union semun {
	int val;
	struct semid_ds* buf;
	unsigned short int* array;
	struct seminfo* __buf;
};

void pv(int sem_id, int op) {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = op;
	sem_b.sem_flg = SEM_UNDO;
	semop(sem_id, &sem_b, 1); //这个1指的是有多少个sem_b
}

int main() {
	int sem_id = semget(IPC_PRIVATE, 1, 0666);// 这个0666和open(), 最后的参数一样的意思。
	union semun sem_un;
	sem_un.val = 1;
	semctl(sem_id, 0, SETVAL, sem_un);

	pid_t id = fork();
	if(id < 0) { 
		return 1;
	} else if (id == 0) {
		printf("child try to get binary sem\n");
		pv(sem_id, -1);
		printf("child get the sem and would release it after 5 seconds\n");
		sleep(5);
		pv(sem_id, 1);
		exit(0);
	} else {
		printf("parent try to get binary sem\n");
		pv(sem_id, -1);
		printf("parent get the sem and would release it after 5 second\n");
		sleep(5);
		pv(sem_id, 1);
	}
	waitpid(id, NULL, 0);
	semctl(sem_id, 0, IPC_RMID, sem_un);
	return 0;


}

