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
 * Interface: HTTP Objects;
 */

/*
 * Getting all objects via HTTP
 * and reconstructing directory hierarchy.
 */
#ifndef GOBJECT_H__
#define GOBJECT_H__

void get_http_object(char *host_name, char *target_location, char *curr_dir);

#endif
