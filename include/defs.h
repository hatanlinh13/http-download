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
 * Interface: Definitions;
 */

/*
 * Definition of macros, types
 * and declaration of global variables.
 */
#ifndef DEFS_H__
#define DEFS_H__

#ifndef NULL
#define NULL 0
#endif

#define MAX_STR_LEN 255

extern int file_count;
extern int http_version;
extern int sockfd;
extern char *prefix;

#endif
