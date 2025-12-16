#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "crawler.h"

int main(int argc, char *argv[]){

	//Arg kontrol
	if (argc < 2){
		printf("Kullanim: %s <path>\n", argv[0]);
	}

	printf("PCheck\n");

	//UnitTest
	char *dir = ".";
	if(argc > 1){
		dir = argv[1];
	}

	TaskQueue q;
	queue_init(&q);

	printf("test basliyo bu klasor taraniyo\n");
	crawl_directory(&q, dir);

	q.finished = 1;
	
	printf("tarandi\n");
	
	while(1){
		char *path = queue_pop(&q);
		if(path == NULL) break;
		printf("bulundu %s\n", path);
		free(path);
	}
	queue_destroy(&q);

	return 0;

}
