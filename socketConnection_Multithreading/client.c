/* simulate client
 * which will make socket  
 * connection to server and port
 * Build: g++ client.c -o client -lnsl -lpthread  //Linux
 * usage :  client <host> <port>
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  //for open
#include <unistd.h>  //for close
#include <pthread.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

/* define structure for host:port values */
typedef struct hostPort{
   struct hostent *server;
   int port;
   char message[2048];
}hostP;

void* clientThread(void* args);

int main(int argc, char *argv[]){

    /*int portno;
    struct hostent *server;*/
    hostP hp;
   
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    hp.port = atoi(argv[2]);

    hp.server = gethostbyname(argv[1]);
    if (hp.server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

   int i=0;
   pthread_t tid[10]; 

   /* create thread */
   while( i<10 ){
      bzero(hp.message,2048);
      sprintf(hp.message, "I am client- thread no. <%d>", i);
      strcat(hp.message, " end\0");
      if(pthread_create( &tid[i], NULL, clientThread, &hp) != 0){
          printf("\nfailed to create thread");
      }
      //sleep(1);
       usleep(1000); //sleep for 1 milliseconds
      i++;
   }

   sleep(2);
   i=0;

   /* join to main thread */
   while(i < 10){
      pthread_join(tid[i++], NULL);
   }

   return 0;
}

void* clientThread(void* args)
{
    hostP threadHP = *((hostP*) args);
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    char buffer[2048];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);    /* create IPv4 connection socket */
    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    /* configure server properties */
    serv_addr.sin_family = AF_INET;
    bcopy((char *)threadHP.server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         threadHP.server->h_length);
    serv_addr.sin_port = htons(threadHP.port);

    /* connect to server on given port */
    if (connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    else
        printf("\nconnected to server <%.30s>:<%d>", (char*)threadHP.server->h_name, threadHP.port);


    /* write to buffer and send over socket */
    //n = write(sockfd,buffer,strlen(buffer));  //Unix
    n=send(sockfd, threadHP.message, strlen(threadHP.message), 0);  //Linux
    if (n < 0) 
         error("ERROR writing to socket");

    /* read from socket   */
    bzero(buffer,2048);
    //n = read(sockfd,buffer,2048);    //Unix
    n=recv(sockfd, buffer, 2048, 0);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("\n%s\n",buffer);
    
   close(sockfd);
   pthread_exit(NULL);
}


