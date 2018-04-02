#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

char buffer[1024];

int main(int argc, char *argv[])
{
	int srv_sock, cli_sock;
	int port_num, ret;
	struct sockaddr_in addr;
	ssize_t len;

	// arg parsing
	if (argc != 2) {
		printf("usage: srv port\n");
		return 0;
	}
	port_num = atoi(argv[1]);

	// socket creation
	srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_sock == -1) {
		perror("Server socket CREATE fail!!");
		return 0;
	}

	// addr binding
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons (INADDR_ANY); // 32bit IPV4 addr that not use static IP addr
	addr.sin_port = htons (port_num); // using port num
	
	ret = bind (srv_sock, (struct sockaddr *)&addr, sizeof(addr));
	
	if (ret == -1) {
		perror("BIND error!!");
		close(srv_sock);
		return 0;
	}
	
	// Listen part
	ret = listen(srv_sock, 0);

	if (ret == -1) {
		perror("LISTEN stanby mode fail");
		close(srv_sock);
		return 0;
	}

	// Accept part ( create new client socket for communicate to client ! )
	cli_sock = accept(srv_sock, (struct sockaddr *)NULL, NULL); // client socket
	
	if (cli_sock == -1) {
		perror("cli_sock connect ACCEPT fail");
		close(srv_sock);
		return 0;
	}
	printf("from client ----\n");
	while (1) {
		memset(buffer, 0, sizeof(buffer));
		len = read(cli_sock, buffer, sizeof(buffer));
		if (len != sizeof(buffer)) { // EOF
			break;
		}
		printf("%s\n len:%d\n", buffer, len);
		//write(cli_sock, buffer, len);
	}
	printf("%s\n len:%d\n", buffer, len);
	printf("----\n");
	sync();
	memset(buffer, 0, sizeof(buffer));
	int count = 10;
	sprintf(buffer, "HTTP/1.1 200 OK\
Date: Mon, 27 Jul 2009 12:28:53 GMT\
Server: Apache/2.2.14 (Win32)\
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\
Content-Length: 88\
Content-Type: text/html\
Connection: Closed\
\r\n\
\r\n\
<html>\
<body>\
<h1>Hello, World!</h1>\
count: %d \
</body>\
</html>\
", count);
	write(cli_sock, buffer, sizeof(buffer));
	printf("%s", buffer);
	sync();
	close(cli_sock);
	return 0;
}
