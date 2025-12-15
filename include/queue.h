#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>

//queuede tutulacak paket (LinkedList Knode)
typedef struct FileTask {
	char *filepath;
	struct FileTask *next;
}FileTask;

typedef struct {
	FileTask *head;	//paket burdan aliniyo
	FileTask *tail;	//paketler buraya ekleniyo
	int count; //bekleyen paket sayisi
	int finished; //1 = end
		      
	pthread_mutex_t mutex; //hocaninistedigi aynianda tek erisim icin kilit
	pthread_cond_t cond; //threadleri uyandirmak icin
} TaskQueue;

//fonksiyon prototypleri (menumuz)
void queue_init(TaskQueue *q);
void queue_push(TaskQueue *q, char *filepath);
char *queue_pop(TaskQueue *q);
void queue_destroy(TaskQueue *q);

#endif

