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
 * Interface: HTTP Version;
 */

/*
 * Setting which HTTP version is in use.
 */
#ifndef HTTPV_H__
#define HTTPV_H__

/*
 * Set global variable *http_version*.
 *
 * Return 1 if successfull set
 * or return 0 if failed to set (invalid command line argument, etc.)
 */
int set_httpv(const char *arg);

#endif
