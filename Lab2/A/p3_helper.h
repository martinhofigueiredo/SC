#ifndef PRIMES_HELPER
#define PRIMES_HELPER

/*
 *  DISCLAIMER: Having code in a C-header file is not recommended. It's not a good coding practice 
 *  for a structured project!
 *  We're doing so to simplify the compilation process, while off-loading the main file.
 */

int is_odd(int num)
{
	return (num % 2 != 0) ? 1 : 0;
}
int is_prime(int num)
{

	if (num == 2) return 1;
	if (num < 2) return 0;

	if (!is_odd(num))
		return 0;
	
	for (int i = 3; i <= (num / 3); i += 2) 
		if (num % i == 0)
			return 0;
	
	return 1;
}
int range_prime_count(int start, int end)
{
	unsigned long primesCount = 0;
	for (int i = start; i != end+1; i++)
		if(is_prime(i))
			primesCount++;

	return primesCount;
}

int prime_count(int end)
{
	return range_prime_count(1, end);
}

#endif

#ifndef JOB_HANDLER
#define JOB_HANDLER

/* *****************************************************************
 * JOB_Handler - A structure to hold the input, result and status
 * ***************************************************************** */

typedef struct {
	unsigned long input_value; //The value we want to compute
	unsigned char processed_flag; // boolean - Flag the result
	unsigned long result; //Stores the result after processing
	unsigned int worker_pid; //Worker process id - to track the process pid.
} JOB_Handler;

/* *****************************************************************
 * A few convenient functions to deal with the JOB_Handler struct
 * ***************************************************************** */

void JOB_init(JOB_Handler *p, unsigned long input_value) {
	p->input_value = input_value;
	p->processed_flag = 0;
	p->worker_pid = 0;
}

void JOB_update_result_and_processing_status_clear(JOB_Handler *p, unsigned long result_value) {
	p->result = result_value;
	p->processed_flag = 1;
	p->worker_pid = 0;
}

void JOB_processing_status_update(JOB_Handler *p, unsigned int worker_pid) {
	p->worker_pid = worker_pid;
}

#endif