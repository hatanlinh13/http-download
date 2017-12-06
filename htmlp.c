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
 * Implementation: HTML Directory Listing;
 */
#include "htmlp.h"
#include "defs.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* copyStr(char* p){
	char tmp[256];
	int i = 0;
	for (; p[i] != '\"'; ++i){
		tmp[i] = p[i];
	}
	tmp[i] = '\0';

	char* res = (char*)calloc(i + 1, sizeof(char));
	strncpy(res, tmp, i + 1);
	return res;
}

int html_parser(char *data, char ***object_list){
	char* tmp = data;
	char* p = NULL;

	p = strstr(tmp, "<title>Index of");
	if (p == NULL)
		return 0;

	p = data;
	tmp = p;
	while ((p = strstr(p, "href=\""))) {
		p += 6;
		if (p[0] != '?')
			break;
	}
	int cnt = 0;
	while ((p = strstr(p, "href=\""))){
		++cnt;
		p += 6;
	}
	*object_list = (char**)calloc(cnt + 1, sizeof(char*));

	p = tmp;
	while ((p = strstr(p, "href=\""))) {
		p += 6;
		if (p[0] != '?')
			break;
	}
	cnt = 0;
	while ((p = strstr(p, "href=\""))){
		p += 6;
		(*object_list)[cnt++] = copyStr(p);
	}
	(*object_list)[cnt] = NULL;

	return 1;
}
