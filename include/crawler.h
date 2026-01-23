#ifndef CRAWLER_H
#define CRAWLER_H
#include "queue.h"

//dir_pathi recursive tariyan ve dosyalari queueye(*q) pushlayan fonksiyon
void crawl_directory(TaskQueue *q, char *dir_path);

void read_from_stdin(TaskQueue *q);

#endif
