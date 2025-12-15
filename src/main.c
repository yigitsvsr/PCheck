#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(int argc, char *argv[]){

	//Arg kontrol
	if (argc < 2){
		printf("Kullanim: %s <path>\n", argv[0]);
		printf("Simdilik test...\n");
	}

	printf("PCheck\n");

	//Thread-pool burda baslatilacak

	//unittest
	TaskQueue q;
	queue_init(&q);
	printf("test1 queueye veri ekleniyor\n");
	queue_push(&q, "/home/test/file1.txt");
	queue_push(&q, "/home/test/file2.txt");
	printf("queueden veri cekiliyor\n");
	char *file1 = queue_pop(&q);
	printf("cekildi 1: %s\n", file1);
	free(file1);
	char *file2 = queue_pop(&q);
	printf("cekildi 2: %s\n", file2);
	free(file2);

	printf("test basarili\n");


	return 0;

}
