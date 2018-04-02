network: srv cli

srv: srv_socket.c
	gcc -o $@ $^

cli: cli_socket.c
	gcc -o $@ $^
