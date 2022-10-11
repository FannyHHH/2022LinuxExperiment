all:client server
client:
	gcc -o client client.c `pkg-config --cflags --libs gtk+-3.0`
server:
	gcc server.c -o server -lpthread
clean:
	rm -f client server
