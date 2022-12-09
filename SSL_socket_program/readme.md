SSL_server.c creates socket server that can continuously listen on input port.
SSL_client.c creates client which can connect to server:port over SSL binding.

Use below commands to compile:
>gcc -Wall -o SSL_server SSL_server.c -L/usr/lib -lssl -lcrypto -lpthread -ld1

>gcc -Wall -o SSL_client SSL_client.c -L/usr/lib -lssl -lcrypto -lpthread -ld1

Use below command to run it:
>./ssl_server <port>

>./ssl_client <server> <port>

I have referred this article to generate self signed SSL certificates for test purpose -
https://help.ubuntu.com/community/OpenSSL 
Give path of certificate & key pem files in program variables or via environment variable.
