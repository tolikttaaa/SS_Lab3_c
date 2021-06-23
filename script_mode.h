//
// Created by ttaaa on 4/22/21.
//
#include "fat32_lib.h"

#ifndef SYSTEM_SOFTWARE_SCRIPT_MODE_H
#define SYSTEM_SOFTWARE_SCRIPT_MODE_H

char *help_command();
char *ls_command(struct partition_value *partition);
int cd_command(struct partition_value *partition, char* to);
int cp_command(struct partition_value *partition, char* source, char* to);
struct partition_value *get_partition(char* path);

#endif //SYSTEM_SOFTWARE_SCRIPT_MODE_H
