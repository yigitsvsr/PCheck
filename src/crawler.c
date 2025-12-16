#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>	//AIdan ogrendim bu lib sayesinde opendir,readdir 
#include <sys/stat.h>	//dosya bilgileri ceken API
#include <unistd.h>	// read write close saglayan API
#include "crawler.h"

void crawl_directory(TaskQueue *q, char *dir_path){
	DIR *dir;
	struct dirent *entry; //dirent:shell dependency azaltmak icin dogrudan posix (linux kernel api) cagrilari
	struct stat file_stat;
	char full_path[1024];

	dir = opendir(dir_path);
	if (dir == NULL){
		perror("Dir acilmadi");
		return;
	}
	
	//dir icini okuyoz
	while ((entry = readdir(dir)) != NULL){

		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
			continue;
		}

		//beyler bu fonksiyonun parametrelerine ve ne ise yaradigina bakin full pathi olusturuyoz
		snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

		if (lstat(full_path, &file_stat) == -1){
			continue;
		}

		if (S_ISDIR(file_stat.st_mode)){

			crawl_directory(q, full_path); //eger nevale DIRse recursif cagri
		}
		else if (S_ISREG(file_stat.st_mode)){

			queue_push(q, full_path); //nevale filesa queueye attik
		}
	}

	closedir(dir);
}
