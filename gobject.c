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
#include "sockmngt.h"
#include "defs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *HTTP_GET = "GET %s HTTP/%s\r\nHost: %s\r\n\r\n";

/* construct HTTP GET message */
char *create_message(char *host_name, char *target_location);
/* get data through socket, return 1 -> html, 2 -> regular file, 0 -> error */
int   get_data(char *http_message, char **data, char *host_name);
/* extract file name from location */
char *create_name(char *target_location);

void get_http_object(
		char *host_name,
		char *target_location,
		char *curr_dir)
{
	char *http_message;
	http_message = create_message(host_name, target_location);

	char *data;
	char **object_list;
	if (get_data(http_message, &data, host_name) == 1) { /* html */
		if (html_parser(data, &object_list) == 1) { /* directory listing */
			char *dir_name = create_name(target_location);

			create_dir(dir_name, curr_dir);
			prefix[0] = '\0';

			int i = 0;
			char *obj = object_list[i];
			while (obj != NULL) {
				char *new_target = (char *)malloc(MAX_STR_LEN);
				char *next_dir   = (char *)malloc(MAX_STR_LEN);

				/* create new target location */
				if (obj[0] == '/') { /* absolue path */
					strncpy(new_target, obj, strlen(obj) + 1);
				}
				else { /* relative path */
					strncpy(new_target,
							target_location,
							strlen(target_location) + 1);
					strncat(new_target, obj, strlen(target_location));
				}
				/* update next directory */
				strncpy(next_dir, curr_dir, strlen(curr_dir) + 1);
				strncat(next_dir, dir_name, strlen(dir_name));
				strncat(next_dir, "/", 1);

				get_http_object(host_name, new_target, next_dir);

				free(new_target);
				free(next_dir);
				i++;
				obj = object_list[i];
			}

			free(dir_name);
		}
		else { /* normal html */
			char *file_name = create_name(target_location);

			save_file(file_name, curr_dir, data);
			prefix[0]  = '\0';

			free(file_name);
		}
	}
	else { /* normal file */
		char *file_name = create_name(target_location);

		save_file(file_name, curr_dir, data);
		prefix[0]  = '\0';

		free(file_name);
	}

	free(http_message);
	free(data);
}

char *create_name(char *target_location)
{
	char *name = (char *)malloc(MAX_STR_LEN);
	/* find last occurrence of / in target location */
	char *anchor = strrchr(target_location, '/');
	/* start copy from the next character */
	anchor += 1;
	strncpy(name, anchor, MAX_STR_LEN);
	return name;
}

char *create_message(char *host_name, char *target_location)
{
	char *message = (char *)malloc(MAX_STR_LEN);
	char version[4];
	if (http_version == 10)
		strncpy(version, "1.0", 4);
	else
		strncpy(version, "1.1", 4);
	sprintf(message, HTTP_GET, target_location, version, host_name);
	return message;
}

int get_data(char *http_message, char **data, char *host_name)
{
	http_message = *data;
	if (10 == http_version) { /* HTTP/1.0 */
		set_up_socket(host_name);

		tear_down_socket();
	}
	else { /* HTTP/1.1 */
	}
	return 0;
}
