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

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE        1000000
#define MAX_HTTP_HEADER 4096
const char *HTTP_GET = "GET %s HTTP/%s\r\nHost: %s\r\n\r\n";

/* release memory used by object list */
void  free_objects(char **object_list);
/* construct HTTP GET message */
char *create_message(char *host_name, char *target_location);
/* extract file name from location */
char *create_name(char *target_location);
/* get data through socket, return 1 -> html, 2 -> regular file, 0 -> error */
int   get_data(char *http_message, char **data, char *host_name, int *len_read);
/* get data using HTTP 1.0 */
int   get_http10_data(char *http_message, char **data, char *host_name, int *len_read);
/* get data using HTTP 1.1 */
int   get_http11_data(char *http_message, char **data, char *host_name, int *len_read);
/* get all HTTP respond header */
int   get_http_header(char *hdr);
/* get data until \r\n
 * used in HTTP 1.1 with chunked encoding
 * to get one line of data at a time */
char *get_line_data();


void get_http_object(char *host_name,
                     char *target_location,
                     char *curr_dir)
{
	char *http_message;
	http_message = create_message(host_name, target_location);

	int len_read;
	char *data = NULL;
	char **object_list = NULL;
	int rd;
	fprintf(stdout, "Getting target %s\n", target_location);
	if ((rd = get_data(http_message, &data, host_name, &len_read)) == 1) { /* html */
		fprintf(stdout, "----Successfully received target.\n");
		if (html_parser(data, &object_list) == 1) { /* directory listing */
			char *dir_name = create_name(target_location);

			if (create_dir(dir_name, curr_dir) == 0) {
				fprintf(stdout, "----Cannot create directory %s. ", dir_name);
				fprintf(stdout, "Skipping this directory.\n");
				free(http_message);
				free(data);
				free_objects(object_list);
				free(dir_name);
				return;
			}
			prefix[0] = '\0';
			fprintf(stdout, "----Created directory: %s\n", dir_name);

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
					strncat(new_target, obj, strlen(obj));
				}
				/* update next directory */
				strncpy(next_dir, curr_dir, strlen(curr_dir) + 1);
				strncat(next_dir, dir_name, strlen(dir_name));

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

			if (save_file(file_name, curr_dir, data, len_read) == 0) {
				fprintf(stdout, "----Cannot save %s to disk. ", file_name);
				fprintf(stdout, "Skipping this file.\n");
				free(http_message);
				free(data);
				free(file_name);
				return;
			}
			prefix[0]  = '\0';
			file_count += 1;
			fprintf(stdout, "----Saved file to disk: %s\n", file_name);

			free(file_name);
		}
	}
	else if (rd == 2) { /* normal file */
		char *file_name = create_name(target_location);

		if (save_file(file_name, curr_dir, data, len_read) == 0) {
			fprintf(stdout, "----Cannot save %s to disk. ", file_name);
			fprintf(stdout, "Skipping this file.\n");
			free(http_message);
			free(data);
			free(file_name);
			return;
		}
		prefix[0]  = '\0';
		file_count += 1;
		fprintf(stdout, "----Saved file to disk: %s\n", file_name);

		free(file_name);
	}
	else { /* Error getting data */
		fprintf(stdout, "----Cannot get data of target: %s. ", target_location);
		fprintf(stdout, "Skipping!\n");
	}

	free(http_message);
	if (data)
		free(data);
}


void free_objects(char **object_list)
{
	if (object_list == NULL)
		return;
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
	int len = strlen(target_location);
	if (target_location[len - 1] == '/') { /* last char */
		target_location[len - 1] = '\0';
		anchor = strrchr(target_location, '/');
		target_location[len - 1] = '/';
	}

	/* start copy from the next character */
	strncpy(name, prefix, strlen(prefix) + 1);
	if (anchor) {
		strncat(name, anchor, strlen(anchor));
		anchor += 1;
	}
	else {
		strncat(name, "noname", 6);
	}

	/* replace all %20 with space */
	char *nameold = name;
	char *namesp = (char *)malloc(MAX_STR_LEN);
	namesp[0] = '\0'; /* empty string */
	char *sp;
	while ((sp = strstr(name, "%20")) != NULL) {
		sp[0] = '\0';
		strcat(namesp, name);
		strcat(namesp, " ");
		name = sp + 3;
	}
	strcat(namesp, name);

	free(nameold);
	return namesp;
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

int get_data(char *http_message, char **data, char *host_name, int *len_read)
{
	if (10 == http_version) /* HTTP/1.0 */
		return get_http10_data(http_message, data, host_name, len_read);
	else /* HTTP/1.1 */
		return get_http11_data(http_message, data, host_name, len_read);
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

int get_http10_data(char *http_message, char **data, char *host_name, int *len_read)
{
	if ((sockfd = set_up_socket(host_name)) == -1) {
		fprintf(stderr, "~~~~Cannot connect to server.\n");
		return 0;
	}

	/* send request */
	if (send(sockfd, http_message, strlen(http_message), 0) == -1) {
		fprintf(stderr, "~~~~Cannot send HTTP request.\n");
		tear_down_socket();
		return 0;
	}

	/* get respond header */
	char respond_hdr[MAX_HTTP_HEADER];
	if (get_http_header(respond_hdr) == 0) {
		fprintf(stderr, "~~~~Cannot get respond header.\n");
		tear_down_socket();
		return 0;
	}

	/* parse header */
	int status_code = 0;
	int num_bytes   = 0;
	int file_type   = 0;
	sscanf(respond_hdr + 9, "%d", &status_code);
	if (status_code != 200) {
		fprintf(stderr, "~~~~Invalid status code: %d.\n", status_code);
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
	if (content_len != NULL) {
		content_len += 16; /* shift pointer points to value */
		sscanf(content_len, "%d", &num_bytes);
	}

	/* get body */
	if (content_len != NULL) { /* had content length specified */
		*data = (char *)malloc(num_bytes);
		int total_read = 0;
		int num_read;
		while ((num_read = recv(sockfd, *data + total_read, num_bytes - total_read, 0)) > 0) {
			total_read += num_read;
		}
		if (total_read != num_bytes) {
			fprintf(stderr, "~~~~Cannot get HTTP respond body.\n");
			tear_down_socket();
			return 0;
		}
		*len_read = total_read;
	}
	else { /* no content length, read until connection closed */
		*data = (char *)malloc(BUF_SIZE);
		int allocated = BUF_SIZE;
		int num_read = 0;
		while (1) {
			if (num_read + 100000 >= allocated) { /* buffer full */
				*data = (char *)realloc(*data, allocated + BUF_SIZE);
				allocated += BUF_SIZE;
			}

			int rd = recv(sockfd, *data + num_read, 100000, 0);
			num_read += rd;
			if (rd <= 0)
				break;
		}
		*len_read = num_read;
	}

	tear_down_socket();
	return file_type;
}

int get_http11_data(char *http_message, char **data, char *host_name, int *len_read)
{
	if ((sockfd = set_up_socket(host_name)) == -1) {
		fprintf(stderr, "~~~~Cannot connect to server.\n");
		return 0;
	}

	/* send request */
	if (send(sockfd, http_message, strlen(http_message), 0) == -1) {
		fprintf(stderr, "~~~~Cannot send HTTP request.\n");
		tear_down_socket();
		return 0;
	}

	/* get respond header */
	char respond_hdr[MAX_HTTP_HEADER];
	if (get_http_header(respond_hdr) == 0) {
		fprintf(stderr, "~~~~Cannot get respond header.\n");
		tear_down_socket();
		return 0;
	}

	/* parse header */
	int status_code = 0;
	int num_bytes   = 0;
	int file_type   = 1;
	sscanf(respond_hdr + 9, "%d", &status_code);
	if (status_code != 200) {
		fprintf(stderr, "~~~~Invalid status code: %d.\n", status_code);
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
	int chunked;
	if (content_len == NULL) { /* chunked encoding */
		chunked = 1;
	}
	else { /* not chunked */
		chunked = 0;
		content_len += 16; /* shift pointer points to value */
		sscanf(content_len, "%d", &num_bytes);
	}

	/* process respond body */
	if (chunked) {
		int chunk_size;
		char *chunk_indicator;
		char *chunk_buffer;

		*data = (char *)malloc(1);
		int data_size = 0;
		while (1) {
			chunk_indicator = get_line_data();
			if (chunk_indicator == NULL) {
				fprintf(stderr, "~~~~Cannot get chunk.\n");
				tear_down_socket();
				return 0;
			}
			sscanf(chunk_indicator, "%x", &chunk_size);

			if (chunk_size > 0) {
				chunk_buffer = get_line_data();
				if (chunk_buffer == NULL) {
					fprintf(stderr, "~~~~Cannot get chunk buffer.\n");
					free(chunk_indicator);
					tear_down_socket();
					return 0;
				}

				*data = (char *)realloc(*data, data_size + chunk_size);
				strncpy(*data + data_size, chunk_buffer, chunk_size);
				data_size += chunk_size;
				free(chunk_buffer);
			}
			else { /* end of respond */
				free(chunk_indicator);
				tear_down_socket();
				*len_read = data_size;
				return file_type;
			}
		}
	}
	else { /* not chunked */
		/* get body */
		*data = (char *)malloc(num_bytes);
		int total_read = 0;
		int num_read;
		while ((num_read = recv(sockfd, *data + total_read, num_bytes - total_read, 0)) > 0) {
			total_read += num_read;
		}
		if (total_read != num_bytes) {
			fprintf(stderr, "~~~~Cannot get HTTP respond body.\n");
			tear_down_socket();
			return 0;
		}
		*len_read = total_read;
	}

	tear_down_socket();
	return file_type;
}

char *get_line_data()
{
	int allocated = BUF_SIZE;
	char *line = (char *)malloc(BUF_SIZE);

	char c1 = '\0';
	char c2 = '\0';
	int num_read = 0;
	while (1) {
		if (num_read == allocated) { /* buffer full */
			line = (char *)realloc(line, allocated + BUF_SIZE);
			allocated += BUF_SIZE;
		}

		if (recv(sockfd, line + num_read, 1, 0) == -1)
			return NULL;

		c1 = c2;
		c2 = line[num_read];
		num_read += 1;

		if (c1 == '\r' && c2 == '\n')
			return line;
	}
}
