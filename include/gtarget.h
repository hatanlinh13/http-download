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
 * Interface: Target Location;
 */

/*
 * Getting file/directory location from URL.
 */
#ifndef GTARGET_H__
#define GTARGET_H__

/*
 * Get the target object location from URL
 *
 * Return 1 on success,
 * or 0 if failed.
 */
int get_target(const char *URL, char *target_location);

#endif
