all: client server

client: Client.o CSV.o HTML.o image_container.o upload_download.o md5sum.o
	     gcc -Wall -o client Client.o CSV.o HTML.o image_container.o upload_download.o md5sum.o -std=c99 -lcrypto

server: Server.o CSV.o image_container.o md5sum.o upload_download.o
	     gcc -Wall -o server Server.o CSV.o image_container.o md5sum.o upload_download.o -std=c99 -lcrypto

Client.o: Client.c
	       gcc -Wall -c Client.c

Server.o: Server.c
	       gcc -Wall -c Server.c

CSV.o: CSV.c
	    gcc -Wall -c -std=c99 CSV.c

HTML.o: HTML.c
	     gcc -Wall -c -std=c99 HTML.c

image_container.o: image_container.c
	                gcc -Wall -c -std=c99 image_container.c

upload_download.o: upload_download.c
	                gcc -Wall -c upload_download.c

md5sum.o: md5sum.c
	       gcc -Wall -c md5sum.c

clean:
	rm -rf *o server client
