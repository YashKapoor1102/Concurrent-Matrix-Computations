/*
Name: Yash Kapoor
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <sys/shm.h>
#include "DET.h"

#define MICRO_SEC_IN_SEC 1000000

int main(int argc, char *argv[]) 
{
	pid_t pid, pid2, pid3;

	struct timeval start, end;
	int m[3][3] = {
		{20, 20, 50}, 			// First row
		{10, 6, 70}, 			// Second Row
		{40, 3, 2}			// Third row
	};	

	void *shared_memory = (void *) 0;
	struct shared_use_st *shared_stuff;
	int shmid;

	printf("\nMatrix program starting\n");

	printf("\nMatrix M = ");
	for(int i = 0; i < 3; i++) {
		// printing the matrix
		printf("\n");
		for(int j = 0; j < 3; j++) {
			printf("%d\t", m[i][j]);
		}
	}

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	// create shared memory

	if (shmid == -1) {
		// error occurs
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	// making shared memory accessible to the program
	shared_memory = shmat(shmid, (void *) 0, 0);

	if (shared_memory == (void *) -1) {
		// error occurs
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	printf("\n\nMemory attached at %X\n", (intptr_t) shared_memory);

	// assigning the shared_memory segment to shared_stuff
	shared_stuff = (struct shared_use_st *) shared_memory;

	gettimeofday(&start, NULL);

	// Creating 3 child processes from a single parent
	(pid = fork()) && (pid2 = fork()) && (pid3 = fork());

	if ((pid == -1) || (pid2 == -1) || (pid3 == -1)) {
		// error occurs while forking
		perror("fork failed");
		exit(1);
	}
	else if (pid == 0) {
		// first child process (P1) calculates D[1] = a * (ei - fh)

		printf("\nChild Process (P1): working with element 1 of D\n");
		int a, max0, determinant;
		a = m[0][0] * ((m[1][1] * m[2][2])-(m[1][2] * m[2][1]));

		shared_stuff->d[0] = a;

		for(int i = 0; i < 1; i++) {
			// finding the maximum value in the first row of the matrix
			max0 = m[i][0];
			for(int j = 1; j < 3; j++) {
				if(m[i][j] > max0) {
					max0 = m[i][j];
				}	
			}
		}
			
		shared_stuff->max[0] = max0;

		// calculating the determinant of the matrix
		determinant = shared_stuff->d[0] + shared_stuff->d[1] + shared_stuff->d[2];
		shared_stuff->result = determinant;

		int n = sizeof(shared_stuff->max) / sizeof(shared_stuff->max[0]);
		for(int j = 1; j < n; j++) {

			// finding the largest element in the whole matrix
			if(shared_stuff->max[0] < shared_stuff->max[j]) {
				// storing the largest number in shared_stuff->max[0]
				shared_stuff->max[0] = shared_stuff->max[j];
			}

			shared_stuff->largest = shared_stuff->max[0];
		}


		// time it takes for all child processes to perform the operations
		gettimeofday(&end, NULL);

		printf("\n\nStart Time: %lf sec\n", start.tv_sec + (double) start.tv_usec/MICRO_SEC_IN_SEC);
		printf("End Time: %lf sec\n", start.tv_sec + (double) end.tv_usec/MICRO_SEC_IN_SEC);

		printf("\nComputational time to perform all the operations: %ld micro sec\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));

	} 
	else if (pid2 == 0) {
		// second child process (P2) calculates D[2] = b * (fg - di)

		printf("\nChild Process (P2): working with element 2 of D");
		int b, max2;
		b = m[0][1] * ((m[1][2] * m[2][0]) - (m[1][0] * m[2][2]));
			
		shared_stuff->d[1] = b;
 
		for(int i = 1; i < 2; i++) {
			// finding the maximum value in the second row of the matrix
			max2 = m[i][0];
			for(int j = 1; j < 3; j++) {
				if(m[i][j] > max2) {
					max2 = m[i][j];
				}	
			}
		}
			
		shared_stuff->max[1] = max2;

	}
	else if (pid3 == 0) {
		// third child process (P3) calculates D[3] = c * (dh - eg)

		printf("\nChild Process (P3): working with element 3 of D");
		int c, max3;
		c = m[0][2] * ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0]));
		shared_stuff->d[2] = c;

		for(int i = 2; i < 3; i++) {
			// finding the maximum value in the third row of the matrix
			max3 = m[i][0];
			for(int j = 1; j < 3; j++) {
				if(m[i][j] > max3) {
					max3 = m[i][j];
				}	
			}
		}

		shared_stuff->max[2] = max3;

	}
	else {
		// parent process

		int stat_val;

		// waiting for all the child processes (P1, P2, P3) to finish 
		pid = wait(&stat_val);
		pid2 = wait(&stat_val);
		pid3 = wait(&stat_val);

		printf("\n\nThe determinant of this matrix is: %d\n", shared_stuff->result);

		printf("\nThe largest value in this matrix is: %d\n", shared_stuff->largest);

		// Shared memory is detached and then deleted 
		if(shmdt(shared_memory) == -1) {
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}
		if (shmctl(shmid, IPC_RMID, 0) == -1) {
			fprintf(stderr, "shmctl(IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}

		if (WIFEXITED(stat_val)) {
			// successful completion of all child processes (P1, P2, P3)  
			// and parent process 
			printf("\n\nAll processes completed successfully.\nDone\n\n");

		}
		else {
			// error occurs 
			printf("\n\nOne of the children terminated abnormally\n\n");
			exit(EXIT_FAILURE);
		}

		exit(EXIT_SUCCESS);

	}
}


	



	
