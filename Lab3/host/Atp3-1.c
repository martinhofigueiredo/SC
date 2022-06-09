#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define READ_BUFFER_SIZE 64
#define MIN_NUMBER_OF_ARGUMENTS 2

static char buffer[READ_BUFFER_SIZE];

int get_file_size(int file_descriptor);
void display_file_content(int file_descriptor);

int main(int argc, char **argv)
{
	char *file_name;
	int file_descriptor;

	if (argc < MIN_NUMBER_OF_ARGUMENTS){
    	fprintf(stderr, "Error: usage: ./cat filename\n");
    	return -1;
	}
	
	file_name = argv[1];

	file_descriptor = open(file_name, O_RDONLY);
	if (file_descriptor == -1){
		fprintf(stderr, "Error: %s: %s\n", file_name, strerror(errno));
    	return -1;
	}
  
	fprintf(stdout, "File '%s' opened (file descriptor with value %d)\n", file_name, file_descriptor);
	fprintf(stdout, "Opened file at offset: %lld\n", lseek(file_descriptor, 0, SEEK_CUR));
	fprintf(stdout, "File size: %d\n", get_file_size(file_descriptor));
	fprintf(stdout, "File contents:\n");
	display_file_content(file_descriptor);

	close(file_descriptor);

	return 0;
}

int get_file_size(int file_descriptor){
	int file_size = 0;
    file_size = lseek(file_descriptor, 0L, SEEK_END);
    // TODO - Obtain file size (in bytes)
	return file_size;
}

void display_file_content(int file_descriptor){
    int read_size;
	lseek(file_descriptor, 0L, SEEK_SET);
    fprintf(stdout, "\n------------------------------------------------------------------------------\n");
	while ((read_size = read(file_descriptor, buffer, READ_BUFFER_SIZE)) > 0){
        // TODO - Display read content on the standard output
		write(STDOUT_FILENO, buffer, read_size);
	}
    fprintf(stdout, "\n------------------------------------------------------------------------------\n");
	
    return;
}