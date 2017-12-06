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

/*
 * Get the object at *target_location* from *host_name*.
 * Check whether the object is a directory or a file.
 *
 * If this is a file then save to *curr_dir* and return.
 *
 * Or if this is a directory, create a directory at *curr_dir*,
 * then set *curr_dir* point to this new directory,
 * then recursively call *get_http_object* for all child objects,
 * finally reset *curr_dir* to its origin value.
 *
 * Files and directories name form: *prefix* + name.
 * Remember to reset *prefix* into empty
 * at the end of first call to *save_file* or *create_directory*
 * (Sub directories and files do not have prefix).
 */
void get_http_object(char *host_name, char *target_location, char *curr_dir);

#endif
