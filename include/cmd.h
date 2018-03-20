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
 * Interface: Command Line Arguments;
 */

/*
 * Parsing and processing command line arguments.
 */
#ifndef CMD_H__
#define CMD_H__

/*
 * Parsing command line arguments
 * get HTTP version, Host Address and Target Location
 * from the given arguments
 * return 0 if failed to parse arguments
 * otherwise return 1
 */
int cmd_parser(int argc, char **argv, char **host_name, char **target_location);

#endif
