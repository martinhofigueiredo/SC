#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "p3_helper.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...) printf("DEBUG: "__VA_ARGS__)
#else
#define DEBUG_PRINTF(...) do {} while (0)
#endif

//The number of jobs to process
#define JOBS_NO 8 

//The number of concurrent workers
#define SIMULTANEOUS_WORKERS_MAX 2

// typedef struct {
// 	unsigned long input_value; //The value we want to compute
// 	unsigned char processed_flag; // boolean - Flag the result
// 	unsigned long result; //Stores the result after processing
// 	unsigned int worker_pid; //Worker process id - to track the process pid.
// } JOB_Handler;

void dispatch_jobs(JOB_Handler jobs[], int number_of_jobs)
{
	//Process all jobs - One process (worker) per job, with
	//a limit on the number of simultaneous workers (SIMULTANEOUS_WORKERS_MAX)
	int status;
	int pid;
	int active_workers = 0;

	//Process all jobs. 
	for (int i=0; i<number_of_jobs; i++){
		int ret;

		if (active_workers == SIMULTANEOUS_WORKERS_MAX){
			//Workers are all busy. Let's wait for some vacant.
			if ((pid = wait(&status))>0){
				DEBUG_PRINTF("Returned from pid: %d with status: %d \n", pid, status);
				active_workers--;
				//Search the pid and save
				for (int i=0; i<number_of_jobs; i++)
					if (pid == jobs[i].worker_pid) {
						JOB_update_result_and_processing_status_clear(jobs+i, status);
						break; //No need to continue searching
					}
			}
		}

		if ( (ret=fork()) == 0){
			// CHILD!
			DEBUG_PRINTF("Worker pid: %d (job: %d with input: %lu)\n", getpid(), i+1, jobs[i].input_value);
			int job_result = prime_count(jobs[i].input_value);	

			exit(job_result); //quits the process immediately from any point in the program 
		}else{
			//The PARENT
			//Save the pid in the job handler - for later reference.
			JOB_processing_status_update(jobs + i, ret /*worker pid*/);
			active_workers++;
		}
	}

	
	while ((pid = wait(&status))>0){
		DEBUG_PRINTF("Returned from pid: %d with status: %d \n", pid, status);
		active_workers--;
		//Search the pid and save
		for (int i=0; i<number_of_jobs; i++)
			if (pid == jobs[i].worker_pid) {
				JOB_update_result_and_processing_status_clear(jobs+i, status);
				break; //No need to continue searching
			}
	}
}

int main(int argc, char const *argv[])
{
	/* 
	 * In  this main function we'll 
	 * 1 - Parametrize a set of job tasks 
	 * 2 - Call the job processing function 
	 * 3 - Print the processing results 
	 */

	JOB_Handler local_var_job_array[JOBS_NO];
	
	//Seeding...
	srand(time(NULL));

	//Fill in some random values
	for (int i=0; i<JOBS_NO; i++)
		JOB_init(local_var_job_array + i, (1+(rand() % 10))*100000 );  //[1..10]*100000
	
	//Process all jobs - blocking function
	dispatch_jobs(local_var_job_array, JOBS_NO);

	//Print the results
	for (int i=0; i<JOBS_NO; i++){
		printf("Job %d, ", i+1);

		if (local_var_job_array[i].processed_flag) 
			printf(" Processed. Result=%lu", local_var_job_array[i].result);
		else 
			printf(" Not Processed.");

		printf("\n");
	}

	return 0;
}
