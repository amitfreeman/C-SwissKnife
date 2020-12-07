/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
   build : gcc server.c -lsocket //on Unix
            g++ server.c -o server -lnsl  //on Linux
   usage : server <port>
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}
int getStopfile();  // #1

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
     serv_addr.sin_port = htons(portno);
     printf("\n now executing bind()");
     /* bind to socket */
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     printf("\nServer is up and running on port <%d>", portno);

     listen(sockfd, 10);   /* listen to socket */
     clilen = sizeof(cli_addr);

     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);   /* accept incoming handshake from client */
         if (newsockfd < 0)
             error("ERROR on accept");
         pid = fork();             /* multiprocessing, fork the process */
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {            /* child process */
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);    /* parent process, close current conn, and listen for new client */

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
void dostuff (int sock)
{
   int n;
   char buffer[256];

   bzero(buffer,256);
   //n = read(sock,buffer,255);     //unix
   n=recv(sock, buffer, 255, 0);    //linux
   if (n < 0) error("ERROR reading from socket");
   printf("\nHere is the message: %s\n",buffer);
   //n = write(sock,"I got your message",18);   //unix
   n=send(sock, "I got your message", 18, 0);    //linux
   if (n < 0) error("ERROR writing to socket");
}

/* check if stop file exists */
int getStopfile(){
   FILE            *fp_stop_file;
   int sts;
   //errno = 0;
   fp_stop_file=fopen("server.stop","r");

   if(  fp_stop_file == NULL)
   {
      /*if (errno == ENOENT)
      {
         /* File does not exists   */
         return 0;  /* SUCCESS = 0   */

      /*}
      else
      {
         printf("Errno %d %s - While open the file %s\n",
               errno,strerror(errno),i_stop_file_name);

         return -1; /* FAILURE = -1   */
      /*}*/
   }
   
  if (fp_stop_file!=NULL)   /* File exists */
  {
     fclose(fp_stop_file);
  }
  return 1;
}
