#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>
#include "common.h"
#include <sys/types.h>	//off_t sagliyo: AIa bu is icin hangi typei kullaniyim dedim bunu önerdi

//queuede tutulacak paket (LinkedList Knode)
typedef struct FileTask {
	char *filepath;
	off_t file_size;
	struct FileTask *next;
}FileTask;

typedef struct {
	FileTask *head;	//paket burdan aliniyo
	FileTask *tail;	//paketler buraya ekleniyo
	int count; //bekleyen paket sayisi
	int finished; //1 = end
	long file_count;
	long byte_count;
	Config *config;
		      
	pthread_mutex_t mutex; //hocaninistedigi aynianda tek erisim icin kilit
	pthread_cond_t cond; //threadleri uyandirmak icin
} TaskQueue;

//fonksiyon prototypleri (menumuz)
void queue_init(TaskQueue *q);
void queue_push(TaskQueue *q, char *filepath, off_t file_size);
char *queue_pop(TaskQueue *q, off_t *p_file_size);
void queue_destroy(TaskQueue *q);
void queue_set_flag(TaskQueue *q);

#endif

