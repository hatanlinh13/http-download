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
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_HTTP_HEADER 4096
const char *HTTP_GET = "GET %s HTTP/%s\r\nHost: %s\r\n\r\n";

/* release memory used by object list */
void  free_objects(char **object_list);
/* construct HTTP GET message */
char *create_message(char *host_name, char *target_location);
/* extract file name from location */
char *create_name(char *target_location);
/* get data through socket, return 1 -> html, 2 -> regular file, 0 -> error */
int   get_data(char *http_message, char **data, char *host_name);
/* get all HTTP respond header */
int   get_http_header(char *hdr);
/* get data using HTTP 1.0 */
int   get_http10_data(char *http_message, char **data, char *host_name);
/* get data using HTTP 1.1 */
int   get_http11_data(char *http_message, char **data, char *host_name);

void get_http_object(char *host_name,
                     char *target_location,
                     char *curr_dir)
{
	char *http_message;
	http_message = create_message(host_name, target_location);

	char *data;
	char **object_list;
	int rd;
	if ((rd = get_data(http_message, &data, host_name)) == 1) { /* html */
		if (html_parser(data, &object_list) == 1) { /* directory listing */
			char *dir_name = create_name(target_location);

			if (create_dir(dir_name, curr_dir) == 0) {
				fprintf(stderr, "Cannot create directory %s. ", dir_name);
				fprintf(stderr, "Skipping this directory.\n");
				free(http_message);
				free(data);
				free_objects(object_list);
				free(dir_name);
				return;
			}
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
			free_objects(object_list);
		}
		else { /* normal html */
			char *file_name = create_name(target_location);

			if (save_file(file_name, curr_dir, data) == 0) {
				fprintf(stderr, "Cannot save %s to disk. ", file_name);
				fprintf(stderr, "Skipping this file.\n");
				free(http_message);
				free(data);
				free(file_name);
				return;
			}
			prefix[0]  = '\0';
			file_count += 1;

			free(file_name);
		}
	}
	else if (rd == 2) { /* normal file */
		char *file_name = create_name(target_location);

		if (save_file(file_name, curr_dir, data) == 0) {
			fprintf(stderr, "Cannot save %s to disk. ", file_name);
			fprintf(stderr, "Skipping this file.\n");
			free(http_message);
			free(data);
			free(file_name);
			return;
		}
		prefix[0]  = '\0';
		file_count += 1;

		free(file_name);
	}
	else { /* Error getting data */
		fprintf(stderr, "Cannot get data for target: %s. ", target_location);
		fprintf(stderr, "Skipping!\n");
	}

	free(http_message);
	free(data);
}

void free_objects(char **object_list)
{
	char *p;
	int i = 0;
	p = object_list[i];
	while (p != NULL) {
		free(p);
		i++;
		p = object_list[i];
	}
	free(p); /* last null element */
	free(object_list);
}

char *create_name(char *target_location)
{
	char *name = (char *)malloc(MAX_STR_LEN);
	/* find last occurrence of / in target location */
	char *anchor = strrchr(target_location, '/');
	/* start copy from the next character */
	anchor += 1;
	strncpy(name, prefix, strlen(prefix) + 1);
	strncat(name, anchor, strlen(anchor));
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
	if (10 == http_version) /* HTTP/1.0 */
		return get_http10_data(http_message, data, host_name);
	else /* HTTP/1.1 */
		return get_http11_data(http_message, data, host_name);
}

int get_http_header(char *hdr)
{
	char c1 = '\0';
	char c2 = '\0';
	char c3 = '\0';
	char c4 = '\0';
	int i;

	i = 0;
	while (recv(sockfd, hdr + i, 1, 0) == 1) {
		c1 = c2;
		c2 = c3;
		c3 = c4;
		c4 = hdr[i];

		if (c1 == '\r' &&
		    c2 == '\n' &&
		    c3 == '\r' &&
		    c4 == '\n')
			return 1;

		i++;
	}

	return 0;
}

int get_http10_data(char *http_message, char **data, char *host_name)
{
	if ((sockfd = set_up_socket(host_name)) == -1) {
		fprintf(stderr, "Cannot connect to server.\n");
		return 0;
	}

	/* send request */
	if (send(sockfd, http_message, strlen(http_message), 0) == -1) {
		fprintf(stderr, "Cannot send HTTP request.\n");
		tear_down_socket();
		return 0;
	}

	/* get respond header */
	char respond_hdr[MAX_HTTP_HEADER];
	if (get_http_header(respond_hdr) == 0) {
		fprintf(stderr, "Cannot get respond header.\n");
		tear_down_socket();
		return 0;
	}

	/* parse header */
	int status_code = 0;
	int num_bytes   = 0;
	int file_type   = 1;
	sscanf(respond_hdr + 9, "%d", &status_code);
	if (status_code != 200) {
		fprintf(stderr, "Invalid status code: %d.\n", status_code);
		tear_down_socket();
		return 0;
	}
	char *content_type = strstr(respond_hdr, "Content-Type:");
	content_type += 14; /* shift pointer points to value */
	if (strncmp(content_type, "text/html", 9) == 0)
		file_type = 1; /* result is html */
	else
		file_type = 2; /* other file type */
	char *content_len = strstr(respond_hdr, "Content-Length:");
	content_len += 16; /* shift pointer points to value */
	sscanf(content_len, "%d", &num_bytes);

	/* get body */
	*data = (char *)malloc(num_bytes);
	if (recv(sockfd, *data, num_bytes, 0) < num_bytes) {
		fprintf(stderr, "Cannot get HTTP respond body.\n");
		tear_down_socket();
		return 0;
	}
	return file_type;

	tear_down_socket();
}

int get_http11_data(char *http_message, char **data, char *host_name)
{
	if ((sockfd = set_up_socket(host_name)) == -1) {
		fprintf(stderr, "Cannot connect to server.\n");
		return 0;
	}

	/* send request */
	if (send(sockfd, http_message, strlen(http_message), 0) == -1) {
		fprintf(stderr, "Cannot send HTTP request.\n");
		tear_down_socket();
		return 0;
	}

	/* get respond header */
	char respond_hdr[MAX_HTTP_HEADER];
	if (get_http_header(respond_hdr) == 0) {
		fprintf(stderr, "Cannot get respond header.\n");
		tear_down_socket();
		return 0;
	}

	/* parse header */
	int status_code = 0;
	int num_bytes   = 0;
	int file_type   = 1;
	sscanf(respond_hdr + 9, "%d", &status_code);
	if (status_code != 200) {
		fprintf(stderr, "Invalid status code: %d.\n", status_code);
		tear_down_socket();
		return 0;
	}
	char *content_type = strstr(respond_hdr, "Content-Type:");
	content_type += 14; /* shift pointer points to value */
	if (strncmp(content_type, "text/html", 9) == 0)
		file_type = 1; /* result is html */
	else
		file_type = 2; /* other file type */
	char *content_len = strstr(respond_hdr, "Content-Length:");
	content_len += 16; /* shift pointer points to value */
	sscanf(content_len, "%d", &num_bytes);

	/* get body */
	*data = (char *)malloc(num_bytes);
	if (recv(sockfd, *data, num_bytes, 0) < num_bytes) {
		fprintf(stderr, "Cannot get HTTP respond body.\n");
		tear_down_socket();
		return 0;
	}
	return file_type;

	tear_down_socket();
}
