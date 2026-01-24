#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open sagliyo
#include <unistd.h> //read close sagliyo
#include <stdint.h>
#include "hasher.h"

//bu degerler proje icin sectigimiz FNV-1a hash algosu icin onemli
//offset baslangic sagliyo. primebitleri dagitiyo
#define FNV_OFFSET 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3
#define BUFFER_SIZE 4096

uint64_t calculate_file_hash(const char *filepath){
	int fd;
	ssize_t bytes_read;
	unsigned char buffer[BUFFER_SIZE];
	uint64_t hash = FNV_OFFSET;

	//Linux API
	fd = open(filepath, O_RDONLY);
	if (fd == -1){
	//	perror("The File could not be opened");
		return 0;
	}

	while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0 ){
		for(ssize_t i = 0; i < bytes_read; i++){     //sizet guvenliK icin bu tur
			
			//FNV-1a algosu xor ve carpma islemleri
			hash = hash ^ buffer[i];
			hash = hash * FNV_PRIME;
		}
	}

	close(fd);

	return hash;

}
