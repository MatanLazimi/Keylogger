connection_type=$1
PREFIX = /usr/local
TCP_SERVER: tcp_server.o files.o tree.o argsList.o
	gcc tcp_server.o -lpthread files.o tree.o argsList.o -o TCP_SERVER

tcp_server.o: tcp_server.c
	gcc -c tcp_server.c

UDP_SERVER: udp_server.o files.o tree.o argsList.o
	gcc udp_server.o -lpthread files.o tree.o argsList.o -o UDP_SERVER	

udp_server.o: udp_server.c
	gcc -c udp_server.c

files.o: files.c
	gcc -c files.c

tree.o: tree.c
	gcc -c tree.c

argsList.o: argsList.c
	gcc -c argsList.c

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin/prj
	cp TCP_SERVER $(DESTDIR)$(PREFIX)/bin/prj/
	cp server-tcp.service /etc/systemd/system/

install-UDP:
	mkdir -p $(DESTDIR)$(PREFIX)/bin/prj
	cp UDP_SERVER $(DESTDIR)$(PREFIX)/bin/prj/
	cp server-udp.service /etc/systemd/system/

clean:
	rm -rf *o UDP_SERVER TCP_SERVER
