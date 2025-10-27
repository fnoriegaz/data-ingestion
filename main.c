#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>


//option is a struct defined in getopt.h
//it helps creating an array of structs of type option
//this is later used by thej getop_long fuction that can take options like:
//--bucket bucket_name or -b bucket_name. pretty reader friendly
struct option long_options[] = {
	{"bucket", required_argument, NULL, 'b'},
	{"cache", required_argument, NULL, 'c'},
	{0, 0, 0, 0}
};


int main(int argc, char *argv[]){
	
	int opt;
	char *bucket_name = (char *)malloc(129*sizeof(char));
	char *cache_file= (char *)malloc(1025*sizeof(char));

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
				printf("getopt already reconized that there is no value for this argument\n");
				str_len = strlen(optarg);
				if(str_len>1){
					memcpy(cache_file,optarg,str_len*sizeof(char));
				}
				break;

			//TODO: keep adding other options! AND FINISH THIS PROGRAM EVEN IF IT TAKES A MONTH!
			default:
				printf("pos no se va a poder\n");
				break;


		}
	}


	//TODO: add logic to check if cache file exists and then open it to read contents.
	if(strlen(cache_file) < 1){
		getcwd(cache_file, 1025*sizeof(char));
	}
	printf("this is cache file: %s\n",cache_file);
	printf("this is bucket name: %s\n",bucket_name);

	return 0;
}
