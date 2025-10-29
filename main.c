#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>
#include<stdbool.h>
#include<pthread.h>
#include <errno.h>


//option is a struct defined in getopt.h
//it helps creating an array of structs of type option
//this is later used by thej getop_long fuction that can take options like:
//--bucket bucket_name or -b bucket_name. pretty reader friendly
struct option long_options[] = {
	{"bucket", required_argument, NULL, 'b'},
	{"cache", required_argument, NULL, 'c'},
	{"source", required_argument, NULL, 's'},
	{"dryrun", required_argument, NULL, 'd'},
	{0, 0, 0, 0}
};


int main(int argc, char *argv[]){
	
	int opt;
	char *bucket_name = (char *)malloc(129*sizeof(char));
	char *cache_file = (char *)malloc(1025*sizeof(char));
	char *sequences_file = (char *)malloc(1025*sizeof(char));
	bool dryrun = false;
	char *sequence_list[1024];
	for(int c=0;c<1024;c++){
		sequence_list[c] = (char *)malloc(1025*sizeof(char));
	}

	if(optarg){
		printf("%s\n", optarg);
	}

	
	while((opt = getopt_long(argc, argv, "b:c:s:", long_options, NULL)) != -1){
		printf("selected option is: %d\n",opt);
		switch(opt){

			case 'b':
				//careful! strlen works because optarg is null terminated
				//otherwise, it might be more difficult
				size_t str_len = strlen(optarg);
				if(str_len>1){
					memcpy(bucket_name,optarg,strlen(optarg)*sizeof(char));
}
				break;

			case 'c':
				str_len = strlen(optarg);
				if(str_len>1){
					memcpy(cache_file,optarg,str_len*sizeof(char));
				}
				break;

			case 's':
				str_len = strlen(optarg);
				if(str_len>1){
					memcpy(sequences_file,optarg,str_len*sizeof(char));
				}
				break;

			case 'd':
				//dry run flag
				str_len = strlen(optarg);
				if(str_len == 1 && !(strcmp(optarg,"yes") || strcmp(optarg,"true")) || strcmp(optarg,"y") || strcmp(optarg,"t") ){
					dryrun = true;
				}
				break;



			//TODO: keep adding other options! AND FINISH THIS PROGRAM EVEN IF IT TAKES A MONTH!
			//TODO: add source file/directory
			//TODO: add thread logic to run aws cli command
			default:
				printf("pos no se va a poder\n");
				break;
		}
	}


	//TODO: add logic to check if cache file exists and then open it to read contents.
	if(strlen(cache_file) < 1){
		getcwd(cache_file, 1025*sizeof(char));
	}
	printf("this is cache file: %s\n", cache_file);
	printf("this is bucket name: %s\n", bucket_name);
	printf("this is sequences file: %s\n", sequences_file);


	//read file line by line
	//each line is read until you find a new line character :)
	FILE *fd = fopen(sequences_file, "r");
	if(! fd){
		printf("whooops, error opening file %s\n", sequences_file);
	}
	else{
		char eof = EOF;
		int line = 0;
		do{
			int line_length = 0;
			int c = 0;
			char chara;
			do{
				//fread(&chara, 1, sizeof(char), fd);
				//sequence_list[line][c] = chara;
				fread(sequence_list[line]+c, 1, sizeof(char), fd);
			}while((char)sequence_list[line][c] != '\n');

		}while(eof != EOF);
		fclose(fd);
	}
	char *aws_ls_args[] = {
		"aws", "s3", "ls", bucket_name, NULL
	};

	execvp("aws", aws_ls_args);
	printf("errno: %d\n", errno);

	return 0;
}
