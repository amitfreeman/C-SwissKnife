/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
   build : gcc server.c -lsocket -lpthread
            g++ server.c -o server -lnsl -lpthread  //on Linux
   usage : server <port>
*/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  //for open
#include <unistd.h>  //for close
#include <pthread.h>

void* dostuff(void* ); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}
int getStopfile();  // #1

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno,  pid; //clilen
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int status;  /* #1 stop file handling */

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);  /* create IPv4 connection socket */
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     /* configure server to listen on port no. */
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);   /* set port no using htons function to use proper byte order */
     printf("\n now executing bind()");
     /* bind to socket */
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     printf("\nServer is up and running on port <%d>", portno);

     listen(sockfd, 10);   /* listen to socket */
     clilen = sizeof(cli_addr);
 
     pthread_t tid[11];
     int i=0;

     while (1) {
         /* accept incoming handshake from client */
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); 
         if (newsockfd < 0) 
             error("ERROR on accept");

         /* for each client request create a thread & assign the client request
          * to it to process so main thread can entertain next request */

         if( pthread_create( &tid[i++], NULL, dostuff, (void*) &newsockfd) != 0 )
            printf("\nfailed to create thread");

         /* join threads to main thread */
         if(i >= 10){
            i=0;
            while(i < 10){
               pthread_join(tid[i++], NULL);
            }

            i=0;
         }

         status = getStopfile();  /* server.stop */
         if(status) {
            printf("\nSTOP FILE FOUND, Exiting\n");
            break;
         }
     } /* end of while */

     return 0; 
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void* dostuff (void* sock)
{
   int n;
   char buffer[1024];
   int tSocket = *((int*) sock);
      
   bzero(buffer,1024);
   //n = read(sock,buffer,1024);     //unix
   n=recv(tSocket, buffer, 1024, 0);    //linux
   if (n < 0) error("ERROR reading from socket");
   printf("\nHere is the message received: %s\n",buffer);
   //n = write(sock,"I got your message",18);   //unix

   /* send message to client socket */
   pthread_mutex_lock(&lock);
   char* message=(char*) malloc(sizeof(buffer)+1024);
   strcpy(message, "Hello Client:<");
    /* calculate end of received buffer */
   //printf("\nDEBUG message <%s>", message);
    int idx=0;
    while(buffer[idx]) { idx++; }
    //printf("\nDEBUG idx <%d>", idx);

   strncat(message, buffer, idx);
   //printf("\nDEBUG1 message <%s>", message);
   strcat(message, ">, I got your message\0");
   //printf("\nDEBUG2 message <%s>", message);
   
   //sleep(1);

   n=send(tSocket, message, strlen(message), 0);    //linux
   if (n < 0) error("ERROR writing to socket");
   
   pthread_mutex_unlock(&lock);
   usleep(1000);  //sleep for 1 mil

   free(message);
   close(tSocket);
   pthread_exit(NULL);
}

/* check if stop file exists */
int getStopfile(){
   FILE            *fp_stop_file;
   int sts;
   //errno = 0;
   fp_stop_file=fopen("server.stop","r");

   if(  fp_stop_file == NULL)
   {
         return 0;  /* SUCCESS = 0   */
   }

  if (fp_stop_file!=NULL)   /* File exists */
  {
     fclose(fp_stop_file);
  }

  return 1;
}
