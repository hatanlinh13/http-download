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
 * Interface: HTML Directory Listing;
 */

/*
 * Parsing HTML file to get listing of a directory.
 */
#ifndef HTMLP_H__
#define HTMLP_H__

/*
 * Parsing a HTML file to check wether
 * this file is a HTTP directory listing or not.
 *
 * If this file is a directory listing,
 * add all child files and directory to *object_list*
 * (*object_list* is a null-terminated list of C string
 * where each entry contain target location of a child object)
 * and return 1.
 *
 * Otherwise (this file is not a directory listing or cannot parse)
 * return 0.
 */
int html_parser(char *data, char ***object_list);

#endif
