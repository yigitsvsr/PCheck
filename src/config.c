#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>   //arg parse icin yardimci
#include "config.h"
#include <unistd.h> // bash -> ./pcheck = pipe mode olmamasi icin eklendi (AI solution)

#define DEFAULT_THREADS 4

static void print_usage(void){
	printf("Usage: ./pcheck [OPTIONS] <target_directory>\n\n");
	printf("Options:\n");
	printf(" -t, --threads <num>	Number of threads to be used (Default: %d)\n", DEFAULT_THREADS);
	printf(" -c, --compare <dir>	Compares with the given directory\n");
	printf(" -h, --help		Help message\n\n");
	exit(1);
}

Config parse_arguments(int argc, char *argv[]){
	Config config;
	//default values
	config.dir_path = NULL;
	config.num_threads = DEFAULT_THREADS;
	config.compare_mode = 0;
	config.compare_path = NULL;

	int opt;
	int option_index = 0;

	static struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"compare", required_argument, 0, 'c'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

//0 program name
	optind = 1;

	while((opt = getopt_long(argc, argv, "t:c:h", long_options, &option_index)) != -1){
		switch(opt){
			case 't':
			         config.num_threads = atoi(optarg);  //atoi aidan ogrendim ise yarar diye
				if(config.num_threads < 1){
					fprintf(stderr, "Error: The number of Threads must be at least 1.\n");
					exit(1);
				}
		         	break;
			case 'c':
				config.compare_mode = 1;
				config.compare_path = optarg;
				break;
			case 'h':
				print_usage();
				break;
			case'?':
				print_usage();
				break;
		}
	}

	if (optind < argc){
		config.dir_path = argv[optind];
	} else{
		if(!isatty(STDIN_FILENO)){ //aidan ogrendim: pipe modu bildiriyo 0 = pipe
			config.dir_path = NULL;
		}
		else{
			fprintf(stderr, "No target directory given and no input from stdin\n\n");
			print_usage();
		}

	}
	return config;
}

void print_banner(Config *config){
	printf("-----PCHECK-----\n");
	if( config->dir_path){
		printf("- Target: %s\n", config->dir_path);
	}
	else{
		printf("- Target: Stdin\n");
	}
	printf("- Threads: %d\n", config->num_threads);
	
	if(config->compare_mode){
		printf("- Compare Mode: ON (Ref: %s)\n", config->compare_path);
	}
	else {
		printf("- Compare Mode: OFF (Hash Calculation)\n");
	}
	printf("--------------------------------------------\n");
}

void print_performance_report(long files, long bytes, double sec){
	printf("\n-----------------------------------------\n");
	printf("\n          Performance Report\n");
	printf("-----------------------------------------\n");

	printf("Duration	: %.3f seconds\n", sec);
	printf("Processed File	: %ld\n", files);
	double size = bytes / (1024.0 * 1024.0);
	printf("Processed Data	: %.2f MB (%ld bytes)\n", size, bytes);
	if(sec > 0.0001){
		printf("Speed	: %.0f file/sec\n", files / sec);
	}else{
		printf("Speed 	: Duration is too short\n");
	}
	printf("-----------------------------------------\n");
}
