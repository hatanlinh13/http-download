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
 * Implementation: Socket Management;
 */
#include "sockmngt.h"
#include "defs.h"

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int set_up_socket(char *host_name)
{
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host_name, "80", &hints, &servinfo) == -1)
		return -1;

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			if (close(sockfd) == -1) {
				freeaddrinfo(servinfo);
				return -1;
			}
			continue;
		}
		break;
	}

	if (p == NULL) {
		freeaddrinfo(servinfo);
		return -1;
	}
	else {
		freeaddrinfo(servinfo);
		return sockfd;
	}
}

int tear_down_socket()
{
	return close(sockfd);
}
