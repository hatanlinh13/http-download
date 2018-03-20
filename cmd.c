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
 * Implementation: Command Line Arguments;
 */
#include "cmd.h"
#include "defs.h"

#include <stdlib.h>
#include <string.h>

/*
int findSubstr(char * pat, char * text){
	for(int i=0; pat[i] != '\0';++i){
		if(pat[i] == text[0]){
            int j;
            for(j=1; text[j] != '\0'; ++j){
	    		if(text[j] != pat[j+i])
                    break;
            }
            if(text[j] == '\0')
                return i + j - 1;
		}
	}
	return -1;
}
int findChar(char * pat, char x){
	for(int i=0; pat[i] != 0; ++i)
		if(pat[i] == x)
			return i;
	return -1;
}
int parseURL(char* arg, char**host_name, char**target_location){
	int n = strlen(arg);
//	printf("%s, len: %d\n", arg, n);
		if(n == 0 ) 
			return 0;
	char *s = (char*)malloc(4);
	strcpy(s, "://");
	s[3] = '\0';
	int pos1 = findSubstr(arg, s);
	int start_host = pos1 + 1;
//	printf("starth: %d\n", start_host);
	if(start_host >= n)
		return 0;

	int hn_len = findChar(arg+start_host, '/');
	int end_host = hn_len + start_host;
	//printf("endh: %d\n", hn_len + start_host);						        
	if(hn_len == -1){
		*host_name = (char*)malloc(n-start_host+1);

		strcpy(*host_name, arg+start_host);
		(*host_name)[n-start_host] = '\0';
		*target_location = (char*)malloc(2);
		(*target_location)[0] = '/';
		(*target_location)[1] = '\0';
		return 1;
	}
	*host_name = (char*)malloc(hn_len + 1);

	strcpy(*host_name, arg+start_host);
	(*host_name)[hn_len] = '\0';

	//printf("host: %s\n", host_name);
	int tg_len = n - end_host;

	*target_location = (char*)malloc(tg_len + 1);

	strcpy(*target_location, arg + end_host);
	(*target_location)[tg_len] = '\0';
	//printf("target: %s\n", target_location);
	return 1;
}
*/

int parseURL(char *arg, char **host_name, char **target_location)
{
	*host_name       = (char *)malloc(MAX_STR_LEN);
	*target_location = (char *)malloc(MAX_STR_LEN);

	if (strlen(arg) == 0)
		return 0;

	char *ihost = strstr(arg, "://");
	if (ihost == NULL)
		ihost = arg;
	else
		ihost += 3;
	char *itrgt = strchr(ihost, '/');
	int host_len;
	if (itrgt == NULL) {
		itrgt = arg + strlen(arg);
		host_len = itrgt - ihost;
		strncpy(*target_location, "/", 1);
		(*target_location)[1] = '\0';
		strncpy(*host_name, arg, host_len);
		(*host_name)[host_len] = '\0';
		return 1;
	}

	host_len = itrgt - ihost;
	strncpy(*host_name, ihost, host_len);
	(*host_name)[host_len] = '\0';
	int targ_len = (arg + strlen(arg)) - itrgt;
	strncpy(*target_location, itrgt, targ_len);
	(*target_location)[targ_len] = '\0';
	return 1;
}

int cmd_parser(int argc, char **argv, char**host_name, char**target_location){
    if(argc == 2){
		http_version = 10;
        return parseURL(argv[1], host_name, target_location);
    }
    else if (argc == 3){
		if (strncmp(argv[1], "--http1.", 8) == 0) {
			if (strncmp(argv[1], "--http1.0", 9) == 0) {
				http_version = 10;
			}
			else if (strncmp(argv[1], "--http1.1", 9) == 0) {
				http_version = 11;
			}
			else {
				return 0;
			}
			return parseURL(argv[2], host_name, target_location);
		}
		else if (strncmp(argv[2], "--http1.", 8) == 0) {
			if (strncmp(argv[2], "--http1.0", 9) == 0) {
				http_version = 10;
			}
			else if (strncmp(argv[2], "--http1.1", 9) == 0) {
				http_version = 11;
			}
			else {
				return 0;
			}
			return parseURL(argv[1], host_name, target_location);
		}
		else {
			return 0;
		}
    }
    else return 0;
}
