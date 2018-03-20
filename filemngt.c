/*
 * MMT - Project 1 - Socket Programming
 * HTTP Download Client
 *
 * 15CNTN  - Team 0xff
 * Members:
 * 1512284 - Ha Tan Linh
 * 1512387 - Do Thanh Nhon
 * 1512491 - Nguyen Thanh Tan
 *
 * Implementation: File Management;
 */
#include "filemngt.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

struct stat st;// = {0};
int create_dir(const char *dir_name, const char *curr_dir){
    
    int dl = strlen(dir_name), cdl = strlen(curr_dir);
    char * tmp = (char*) malloc(dl + cdl + 1);
    strcpy(tmp, curr_dir);
    strcpy(tmp+cdl, dir_name);
    tmp[cdl+dl] = '\0';

/*
    if (stat(tmp, &st) == -1) {
        mkdir(tmp, 0755);
		return 1;
    }
	return 1;
*/
	if (stat(tmp, &st) == -1) {
        if(mkdir(tmp, 0755) == 0)
                return 1;
        return 0;
    }
    else if ((stat(tmp, &st) == 0) && (((st.st_mode) & S_IFMT) == S_IFDIR))
        return 1;
    return 0;
}

struct stat f;// = {0};
int save_file(const char *file_name, const char *curr_dir, char *data, int len_write) {
/* no need, curr_dir existence is guaranteed
    if (stat(curr_dir, &f) == -1) {
        mkdir(curr_dir, 0777);
//		printf("file");
    }
*/
    int dl = strlen(file_name), cdl = strlen(curr_dir);
    char * tmp = (char*)malloc(dl + cdl + 1);
    strcpy(tmp, curr_dir);
    strcpy(tmp+cdl, file_name);
    tmp[cdl+dl] = '\0';
//	printf("%s\n", tmp);
    FILE *fp;
    fp = fopen(tmp, "wb");
    if (fp == NULL){
        printf("Cannot open file\n");
        return 0;
    }
    //int result = fputs(data, fp);
	int result = fwrite(data, sizeof(char), len_write, fp);
    if (result != len_write){
        printf("Writing file failed!!!\n");
		fclose(fp);
        return 0;
    }
	free(tmp);
    fclose(fp);
	return 1;
}
