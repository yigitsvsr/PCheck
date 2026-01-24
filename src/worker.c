#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include "worker.h"
#include "hasher.h"
#include "queue.h"
#include "common.h"

const char* get_status_str(uint64_t hash1, uint64_t hash2){
	if(hash2 == 0) return "NEW";
	if(hash1 == hash2) return "UNCHANGED";
	return "MODIFIED";
}

void *start_worker(void *arg){
	TaskQueue *q = (TaskQueue *)arg;
	Config *cfg = q->config;
	char other_path[1024];
	off_t file_size = 0;

	while(1){
		char *path = queue_pop(q, &file_size); //queue bossa queuepop threadi uyutuyo

		if(path == NULL){
			break;
		}

		pthread_mutex_lock(&q->mutex);
		q->file_count++;
		q->byte_count += file_size;
		pthread_mutex_unlock(&q->mutex);

		uint64_t original_hash = calculate_file_hash(path);

		//Compare mode 0
		if(cfg->compare_mode == 0){
		printf("%016" PRIx64 "\t%s\n", original_hash, path);
		}
		else{
			//Compare mode 1
			size_t length = strlen(cfg->dir_path);
			const char *relative_path = path + length;
			snprintf(other_path, sizeof(other_path), "%s%s", cfg->compare_path, relative_path); //path merge
		
			uint64_t other_hash = calculate_file_hash(other_path);
			const char *status = get_status_str(original_hash, other_hash);

			//havali bastirma aia sordum ogrendim
			if(strcmp(status, "UNCHANGED") == 0){
				printf("\033[32m[UNCHANGED]\033[0m	%s\n", path);
			}else if(strcmp(status, "MODIFIED") == 0){
				printf("\033[31m[MODIFIED]\033[0m	%s\n", path);
			}else {
				printf("\033[33m[NEW]\033[0m		%s\n", path);
			}
		}


		free(path);
	
	}
	return NULL;
}
