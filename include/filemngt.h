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
 * Interface: File Management;
 */

/*
 * Creating directory, files and copying data to files.
 */
#ifndef FILEMNGT_H__
#define FILEMNGT_H__

/*
 * Create a file at *curr_dir* with *file_name*
 * and save *data* to this file
 *
 * Return 1 on success
 * or 0 if failed
 */
int save_file(const char *file_name, const char *curr_dir, char *data, int len_write);

/*
 * Create a directory *dir_name* at *curr_dir*
 *
 * Return 1 on success
 * or 0 if failed
 */
int create_dir(const char *dir_name, const char *curr_dir);

#endif
