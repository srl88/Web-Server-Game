#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main( int argc, char *argv[]){

  int serversock, status;
  struct sockaddr_in server;
  struct hostent *hp;
  char buff[1024];
  char decision;
  int port;

  /*checks for user input for connection*/
  if (argc != 3) {
    fprintf(stderr, "Usage: client hostname\n");
    exit(1);
  }
  /*socket creation*/
  serversock = socket(AF_INET, SOCK_STREAM, 0);
  if (serversock==-1){perror("Socket failed\n"); exit(1);}

  hp = gethostbyname(argv[1]);

  /*checks if portname is valid*/
  if (hp==0){perror("gethostbyname failed\n"); exit(1);}

  /*checks if port number is valid*/
  if ((port = atoi(argv[2]))==0){perror("Failed port number\n"); exit(1);}

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr = *((struct in_addr *) hp->h_addr);

  if (connect (serversock, (struct sockaddr *) &server, sizeof(server)) < 0){
    perror("Connection Failed\n"); exit(1);
  }
  /*starts playing the game*/
  while(1){
    printf("Please enter your decision: S to be silent, B to betray or Q to quit:\n");
    scanf(" %s", buff);
    /*checks if user input is valid*/
    if((buff[0]=='S'||buff[0]=='B')&&(strlen(buff)==1)){
      /*send the data and checks for proper delivery*/
        if (send(serversock, buff, sizeof(buff), 0) < 0){
          perror("send failed\n"); close(serversock); exit(1);
        }
        else{
          /*waits for answer from server*/
          status = recv(serversock, buff, sizeof(buff), 0);
          if(status<=0){ printf("Connection closed or error\n"); break;}
          buff[status]='\0';
          printf("\nFinal sentence:\n%s", buff);
        }
    }
    else if(buff[0]=='Q' && strlen(buff)==1){printf("You chose to quit the game.\n"); return 0;}
    else{
      printf("Please enter the proper input.\n");
    }
  }
  /*closes socket*/
  close(serversock);
  return 0;
}
