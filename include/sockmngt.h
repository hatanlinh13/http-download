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
 * Interface: Socket Management;
 */

/*
 * Setting up and tearing down socket connections.
 */
#ifndef SOCKMNGT_H__
#define SOCKMNGT_H__

int set_up_socket(char *host_name);
void tear_down_socket();

#endif
