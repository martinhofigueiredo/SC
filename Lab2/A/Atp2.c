#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int some_global_var = 0;

int main()
{
    int some_local_var = 0;
    int ret;
    if ( (ret=fork()) == 0){
        // child process running here because return fork returned zero
        printf("Child(%d) says: Hello!\n", getpid());

        printf("Child(%d) says: Global (%p) incremented to %d!\n", getpid(), &some_global_var, ++some_global_var );
        printf("Child(%d) says: Local (%p) incremented to %d!\n", getpid(), &some_local_var, ++some_local_var );

        printf("Child(%d) says: Done!\n", getpid());
        
        exit(0); //quits the process immediatly from any point in the program 
    }
    else{
        // parent process running here because fork returned value non-zero.
        printf("Parent says: My child's id: %d!\n", ret);

        //Wait until one child exits. NULL means I don't care about the child's exit code.
        int c = wait(NULL);
        printf("Parent says: Child %d is done and so am I!\n", c);

        printf("Parent says: Global (%p) is %d!\n", &some_global_var, some_global_var);
        printf("Parent says: Local (%p) is %d!\n", &some_local_var, some_local_var);
    }

    return 0;
}
