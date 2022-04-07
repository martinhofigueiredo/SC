#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Exercise:  Make heavy_load() last 15s every time DONE

#define HOW_MANY_FORKS 4 //Number of forks. Testing from 1 to 4 should be enough
#define ENABLE_HEAVY_LOAD
#define BUSY_WAIT_S 15

unsigned long _timestamp_sec(clockid_t clk_id){
    struct timespec tp;
    clock_gettime(clk_id, &tp);
    return tp.tv_sec;
}

void heavy_load() {
    unsigned long process_ts_start = _timestamp_sec(CLOCK_PROCESS_CPUTIME_ID);
    while(_timestamp_sec(CLOCK_PROCESS_CPUTIME_ID)-process_ts_start < BUSY_WAIT_S){
    
    }

}

int main() {
  
    printf("Starting with process id: %d\n", getpid());

    for (int i=0; i<HOW_MANY_FORKS; i++) 
        fork();
  
    printf("Running process id: %d\n", getpid());

#ifdef ENABLE_HEAVY_LOAD
    {
        unsigned long process_ts_start = _timestamp_sec(CLOCK_PROCESS_CPUTIME_ID);
        unsigned long system_ts_start = _timestamp_sec(CLOCK_REALTIME);

        heavy_load(); 

        printf("Done with pid: %d in (%lu process seconds) and (%lu system seconds).\n", 
            getpid(), 
            _timestamp_sec(CLOCK_PROCESS_CPUTIME_ID)-process_ts_start,
            _timestamp_sec(CLOCK_REALTIME)-system_ts_start
        );
    }
#endif

    return 0;
}