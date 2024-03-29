#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include "/usr/include/openssl/ssl.h"
#include "/usr/include/openssl/crypto.h"
#include "/usr/include/openssl/err.h"

#define FAIL    -1
char *ssl_cert_file="/home/qarun/aparopka/myCA/server_crt.pem";
char *ssl_key="/home/qarun/aparopka/myCA/server_key.pem";

//Added the LoadCertificates how in the server-side makes.
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
 /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_chain_file(ctx, CertFile ) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    char *imethod=getenv("TLS_METHOD");
    if(imethod!=NULL && strncmp(imethod, "1.3", 3)==0){
       printf("\nSetting TLS1.3 method\n");
       fflush(stdout);
       method = TLS_client_method();  /* create new server-method instance */
    }
    else{
       printf("\nSetting TLS1.2 method\n");
       fflush(stdout);
       method = TLSv1_2_client_method();  /* create new server-method instance */
    }

    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if(imethod!=NULL && strncmp(imethod, "1.3", 3)==0)
        SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 | SSL_OP_NO_TLSv1_2);

    return ctx;
}

void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}

int main(int count, char *strings[])
{   SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    int bytes;
    char *hostname, *portnum;

    if ( count != 3 )
    {
        printf("usage: %s <hostname> <portnum>\n", strings[0]);
        exit(0);
    }
    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];

    ctx = InitCTX();
    LoadCertificates(ctx, ssl_cert_file, ssl_key);
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    int sd = SSL_get_fd(ssl);
    int flip=1;
    printf("\nsd: <%d>", sd);
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {   //char *msg = "Hello???";
        if(SSL_do_handshake(ssl) <= 0){
            printf("\n client SSL_do_handshake failed....\n");
        }
        char msg[1024];
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);        /* get any certs */
        while(1){
                bzero(msg, 1024);
                bzero(buf, 1024);
                printf("\nEnter message: ");
                fgets(msg, 1024, stdin);
                SSL_write(ssl, msg, strlen(msg));   /* encrypt & send message */
                //send(sd, msg, strlen(msg),  0); //MSG_NOSIGNAL | MSG_DONTWAIT); //MSG_CONFIRM);

                bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
                //bytes = read(sd, buf, sizeof(buf));
                //bytes = recv(sd, buf, sizeof(buf), 0);

                  /* read again for TLS 1.3 only for first time */
                  /*char *imethod=getenv("TLS_METHOD");
                  if(imethod!=NULL && strncmp(imethod, "1.3", 3)==0
                       && flip){
                      bytes = read(sd, buf, sizeof(buf));
                      flip=0;
                  }*/

                buf[bytes] = 0;
                printf("Received: \"%s\"\n", buf);
        }
        SSL_free(ssl);        /* release connection state */
    }
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}
