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
 * Module: Main Routine;
 */
#include "cmd.h"
#include "gobject.h"
#include "sockmngt.h"
#include "defs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int file_count;   /* number of files downloaded */
int http_version; /* 10 for HTTP 1.0
                     and 11 for HTTP 1.1 */
int sockfd;       /* socket file descriptor */
char *prefix;     /* level 1 file/directory name prefix,
                     empty for sub-directories and files */

int
main(int argc, char **argv)
{
	char *host_name;
	char *target_location;
	/* current location of the file being downloaded */
	char *curr_dir = (char *)malloc(MAX_STR_LEN);
	prefix         = (char *)malloc(MAX_STR_LEN);

	if (cmd_parser(argc, argv, &host_name, &target_location) == 0) {
		fprintf(stderr, "Invalid command line argument!\n");
		fprintf(stdout, "Usage: exe <http version> <target URL>\n");
		exit(EXIT_FAILURE);
	}

	strncpy(curr_dir, "./", 3);
	strncpy(prefix, "1512284_1512387_1512491_", 25);
	file_count = 0;
	get_http_object(host_name, target_location, curr_dir);
	fprintf(stdout, "Finished operation. Downloaded %d files.\n", file_count);

	free(host_name);
	free(target_location);
	free(curr_dir);
	free(prefix);
	exit(EXIT_SUCCESS);
}
