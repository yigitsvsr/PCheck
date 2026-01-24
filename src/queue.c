#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"


void queue_init(TaskQueue *q) {
	q->head = NULL;
	q->tail = NULL;
	q->count = 0;
	q->finished = 0;
	q->config = NULL;
	q->file_count = 0;
	q->byte_count = 0;
	//kilit ve zil mekanizmalari
	pthread_mutex_init(&q->mutex, NULL);
	pthread_cond_init(&q->cond, NULL);
}

//Add.  Crawler bu methodu kullanicak haberiniz olsun
void queue_push(TaskQueue *q, char *filepath, off_t file_size) {
	FileTask *new_task = malloc(sizeof(FileTask)); //1- Yeni node icin mallocla memory allocate
	if (!new_task) {
		perror("Memory allocation failed in 'push'");
		exit(1);
	}
	//2- dosya yolunu kopyalama islemi 
	//ONEMLI AI TARAFINDAN: strdup onerildi ve kullanildi. pointer degisebilecegi icin bundan kaynakli hata almamak amaciyla
	new_task->filepath = strdup(filepath);
	new_task->file_size = file_size;
	new_task->next = NULL;

	//Kilitleme islemi
	pthread_mutex_lock(&q->mutex);

	if(q->tail == NULL) {
		q->head = new_task;
		q->tail = new_task;
	} else {
		q->tail->next = new_task;
		q->tail = new_task;
	}
	q->count++;

	pthread_cond_signal(&q->cond); //uyuyan thread varsa uyandir
			
	pthread_mutex_unlock(&q->mutex); //kilidi ac
}

//Queueden dosya cekme islemi. Consumer ve Worker kullanacak
//queue bossa data gelene kadar blocking
char *queue_pop(TaskQueue *q, off_t *p_file_size) {
	pthread_mutex_lock(&q->mutex);

	//finished flag 1 olana kadar bekle sonra donguden cik
	while (q->count == 0 && !q->finished) {
		pthread_cond_wait(&q->cond, &q->mutex);
	}                  

	//is bittiyse ve queue bossa null dondur ve workeri kapa
	if (q->count == 0 && q->finished) {
		pthread_mutex_unlock(&q->mutex);
		return NULL;
	}

	FileTask *task = q->head; //queueden eleman al
	char *result_path = task->filepath;

	if(p_file_size){
		*p_file_size = task->file_size;
	}

	q->head = task->next;
	if (q->head == NULL){
		q->tail = NULL;
	}
	q->count--;

	free(task); //burda result_pathi freelemiyorum taski freeliyorum cunku onu worker kullanip freelicek
	
	pthread_mutex_unlock(&q->mutex);
	return result_path;
}

//some cleanin
void queue_destroy(TaskQueue *q) {

	//eger queuede eleman kalmissa inlari temizliyoruz tamamen guvenlik amacli ai onero
	while (q->head != NULL){
		FileTask *temp = q->head;
		q->head = temp->next;
		free(temp->filepath);
		free(temp);
	}

	pthread_mutex_destroy(&q->mutex);
	pthread_cond_destroy(&q->cond);
}

void queue_set_flag(TaskQueue *q){
	pthread_mutex_lock(&q->mutex);
	q->finished = 1;
	pthread_cond_broadcast(&q->cond);
	pthread_mutex_unlock(&q->mutex);
}
