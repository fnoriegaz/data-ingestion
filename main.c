#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>
#include<stdbool.h>
#include<pthread.h>
#include <errno.h>


/*
 * should call aws s3 rync local to s3 bucket.
 * should be able to select specific frames to upload.
 * chache file might not be necessary if rsync is executed(?) pending to confirm.
 * if frames are not specified, default is to sync whole sequence.
 * */


//option is a struct defined in getopt.h
//it helps creating an array of structs of type option
//this is later used by thej getop_long fuction that can take options like:
//--bucket bucket_name or -b bucket_name. pretty reader friendly
struct option long_options[] = {
	{"bucket", required_argument, NULL, 'b'},
	{"cache", required_argument, NULL, 'c'},
	{"source", required_argument, NULL, 's'},
	{"dryrun", required_argument, NULL, 'd'},
	{"frames", required_argument, NULL, 'f'},
	{0, 0, 0, 0}
};


int main(int argc, char *argv[]){
	
	int opt;
	int n_sequences = 0;
	int n_frames = 0;
	char *bucket_name = (char *)malloc(257*sizeof(char));
	char *cache_file = (char *)malloc(1025*sizeof(char));
	char *frames_file = (char *)malloc(1025*sizeof(char));
	char *sequences_file = (char *)malloc(1025*sizeof(char));
	bool dryrun = false;
	char *sequence_list[1024];
	char *frames_list[1024];
	for(int c=0;c<1024;c++){
		sequence_list[c] = (char *)malloc(1025*sizeof(char));
	}
	for(int c=0;c<1024;c++){
		frames_list[c] = (char *)malloc(1025*sizeof(char));
	}

	if(optarg){
		printf("%s\n", optarg);
	}

	
	while((opt = getopt_long(argc, argv, "b:c:s:", long_options, NULL)) != -1){
		printf("selected option is: %d\n",opt);
		switch(opt){

			case 'b':
				//careful! strlen works because optarg is null terminate
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
				if( !(strcmp(optarg,"yes") || strcmp(optarg,"true")) || strcmp(optarg,"y") || strcmp(optarg,"t") ){
					dryrun = true;
				}
				break;
			
			case 'f': //f to pay respects
				str_len = strlen(optarg);
				if(str_len>1){
					memcpy(frames_file,optarg,str_len*sizeof(char));
				}
				break;

			//TODO: keep adding other options! AND FINISH THIS PROGRAM EVEN IF IT TAKES A MONTH!
			default:
				printf("pos no se va a poder\n");
				break;
		}
	}

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
		printf("whooops, error opening sequences file \n");
	}
	else{
		while(fgets(sequence_list[n_sequences], 1024, fd)){
			printf("%s",sequence_list[n_sequences]);
			n_sequences++;
		}
		fclose(fd);
	}

	fd = fopen(frames_file, "r");
	if(!fd){
		printf("whooops, error opening frames file\n");
	}
	else{
		while(fgets(frames_list[n_sequences], 1024, fd)){
			printf("%s",frames_list[n_frames]);
			n_frames++;
		}
		fclose(fd);
	}

	for(int c=0;c<n_sequences;c++){
		char *s3_url = (char *)malloc(1024*sizeof(char));
		int bucket_length = strlen(bucket_name);
		int sequence_length = strlen(sequence_list[c]);
		char *aws_args[7];
		memcpy(s3_url,bucket_name,bucket_length*sizeof(char));
		memcpy(s3_url+bucket_length,sequence_list[c],sequence_length*sizeof(char));
	
		if(dryrun){
			aws_args[0] ="aws";
			aws_args[1] = "s3";
			aws_args[2] = "sync";
			aws_args[3] = "--dryrun";
			aws_args[4] = s3_url;
			aws_args[5] = s3_url;
			aws_args[6] = NULL;
		}
		else{
			aws_args[0] ="aws";
			aws_args[1] = "s3";
			aws_args[2] = "sync";
			aws_args[3] = s3_url;
			aws_args[4] = s3_url;
			aws_args[5] = NULL;
		}

		
		int pid = fork();
		if(0 == pid){
			//as a reminder, this block is executed by child process
			printf("running aws cmd:\n");
			for(int c=0;c<6;c++){printf("arg %d: %s ",c, aws_args[c]);}
			printf("\n");
			execvp("aws", aws_args);
		}
		else{
			if(errno)printf("errno: %d\n", errno);
		}
	}


	return 0;
}
