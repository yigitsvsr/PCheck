#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <time.h>
#include "queue.h"
#include "crawler.h"
#include "hasher.h"
#include "worker.h"
#include "common.h"
#include "config.h"

#define DEFAULT_THREADS 4

int main(int argc, char *argv[]){

	//TestCase ler icin timer (ZAMAN OLCMEYI AI KURDU)
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	Config config = parse_arguments(argc, argv);	
	print_banner(&config);

	//queueyi hazirla
	TaskQueue q;
	queue_init(&q);
	q.config = &config;
	
	//threadleri olustur
	pthread_t *threads = malloc(sizeof(pthread_t) * config.num_threads);
	for(int i = 0; i < config.num_threads; i++) {
		if(pthread_create(&threads[i], NULL, start_worker, &q) != 0){
			perror("Thread Error");
			exit(1);
		}
	}

	//path verilmediyse stdinden oku
	if(config.dir_path == NULL){
		printf("[INFO] Reading stdin...\n");
		read_from_stdin(&q);
	}
	else{
	//main thread dosyalari bulsun ve queueye atsin
	crawl_directory(&q, config.dir_path);
	}

	//bu blok sayesinde threadlere artik queueye baska is gelmicek bu yüzden elinizdeki isleri bitirince kapanmaya hazirlanin cagirisi yapiyoruz
	queue_set_flag(&q);
	
	// join ile maine ekledik üstteki cagridan sonra isler bitince join yapisi ile güvenli bir sekilde kapanicaklar
	for(int i = 0; i < config.num_threads; i++){
		pthread_join(threads[i], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &end_time);
	double sec = end_time.tv_sec - start_time.tv_sec;
	sec += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

	print_performance_report(q.file_count, q.byte_count, sec);
	
	free(threads);
	queue_destroy(&q);

	return 0;
}
