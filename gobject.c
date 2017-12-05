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
 * Implementation: HTTP Objects;
 */
#include "gobject.h"
#include "htmlp.h"
#include "filemngt.h"
#include "defs.h"

#include <stdlib.h>

char *create_message(char *host_name, char *target_location);
int get_data(int sockfd, char *http_message, char **data);

void get_http_object(
		int   sockfd,
		char *host_name,
		char *target_location,
		char *curr_dir)
{
	char *http_message;
	http_message = create_message(host_name, target_location);

	char *data;
	if (get_data(sockfd, http_message, &data) == 1) { /* html */
		//?
	}
	else { /* normal file */
		char *file_name = (char *)malloc(MAX_STR_LEN);
		/* construct file name from prefix and target_location */
		//?
		save_file(file_name, curr_dir, data);
		if (prefix[0] != '\0') /* first time save file or create directory */
			prefix[0]  = '\0';
	}

	free(http_message);
	free(data);
}
