#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

Config parse_arguments(int argc, char *argv[]);
void print_banner(Config *config);
void print_performance_report(long files, long bytes, double sec);

#endif
