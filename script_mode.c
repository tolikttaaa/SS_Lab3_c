//
// Created by ttaaa on 4/22/21.
//

#include <stdlib.h>
#include "stdio.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "fat32_lib.h"
#include "script_mode.h"
#include "utils.h"

char *print_dir(struct dir_value *pValue) {
    char* res = "";

    while (pValue != NULL) {
        if (pValue->type == 'd') {
            res = concat(res, "DIR ");
        } else {
            res = concat(res, "FILE ");
        }

        res = concat(res, (char*) pValue->filename);
        res = concat(res, "\n");

        pValue = pValue->next;
    }

    return res;
}

void copy_file(struct partition_value *part, char *destination, struct dir_value *file) {
    if (file->type != 'f') {
        fprintf(stderr, "Not a file\n");
        return;
    }

    char *buf = malloc(part->cluster_size);
    unsigned int fat_record = file->first_cluster;
    int fd = open(destination, O_RDWR | O_APPEND | O_CREAT, 0777);
    unsigned int size = file->size < part->cluster_size ? file->size : part->cluster_size;

    while (fat_record < 0x0FFFFFF7) {
        fat_record = read_file_cluster(part, fat_record, buf);
        write(fd, buf, size);
    }

    free(buf);
    close(fd);
}

void copy_dir(struct partition_value *part, char *destination, struct dir_value *file) {
    if (file->type != 'd') {
        fprintf(stderr, "Not a dir\n");
        return;
    }

    struct stat dir = {0};
    if (stat(destination, &dir) == -1) {
        mkdir(destination, 0777);
    }

    struct dir_value *dir_val = read_dir(file->first_cluster, part);
    while (dir_val != NULL) {
        if (strcmp((char*)  dir_val->filename, ".") != 0 && strcmp((char*) dir_val->filename, "..") != 0) {
            char *path = calloc(1, 512);
            strcat(path, destination);
            append_path_part(path, (char*) dir_val->filename);

            if (dir_val->type == 'd') {
                copy_dir(part, path, dir_val);
            } else {
                copy_file(part, path, dir_val);
            }

            free(path);
        }

        dir_val = dir_val->next;
    }
    destroy_dir_value(dir_val);
}

char *help_command() {
    char *msg = "cd [arg] - change working directory\n";
    msg = concat(msg, "pwd - print working directory full name\n");
    msg = concat(msg, "cp [arg] [arg] - copy dir or file to mounted device\n");
    msg = concat(msg, "ls - show working directory elements\n");
    msg = concat(msg, "exit - terminate program\n");
    msg = concat(msg, "help - print help\n");
    return msg;
}

char *ls_command(struct partition_value *partition) {
    struct dir_value *dir_value = read_dir(partition->active_cluster, partition);
    char *res = print_dir(dir_value);
    destroy_dir_value(dir_value);
    return res;
}

int cd_command(struct partition_value *partition, char *to) {
    if (change_dir(partition, (unsigned char*) to)) {
        return 1;
    } else {
        return 0;
    }
}

int cp_command(struct partition_value *partition, char* source, char* to) {
    struct dir_value *dir_value = read_dir(partition->active_cluster, partition);

    while (dir_value != NULL) {
        if (!strcmp((char*) dir_value->filename, source)) {
            if (check_directory(to)) {
                char filename[256] = {0};
                strcpy(filename, to);
                size_t str_len = strlen(to);

                if (filename[str_len - 1] != '/') {
                    strcat(filename, "/");
                }

                strcat(filename, (char *) dir_value->filename);

                if (dir_value->type == 'd') {
                    copy_dir(partition, filename, dir_value);
                } else {
                    copy_file(partition, filename, dir_value);
                }

                return 0;
            } else {
                return 2;
            }
        }

        dir_value = dir_value->next;
    }

    destroy_dir_value(dir_value);
    return 1;
}

void parse_partition(struct partition_value *partition, char *path) {
    while (strlen(path) > 0) {
        char *dir = get_before(path, '/');
        cd_command(partition, dir);
        remove_before(path, '/');
    }
}

struct partition_value *get_partition(char* path) {
    const char *part = get_before(path, '/');

    struct partition_value *partition = open_partition(part);

    remove_before(path, '/');

    parse_partition(partition, path);
    return partition;
}
