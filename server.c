#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

/*
Method to produce 1 or 0 randomly
 */
int randomAnswer(){
  return (rand()%2);
}
/*
Method to calculate the total sentance for both players. returns a final message.
 */
void  sentence(char A, int  B, char *msg){
  if(A=='S'&&B==1){
    strcpy(msg, "The server chooses to be silent.\nClient gets 1 year and Server gets 1 year.\n");
  }
  else if(A=='S'&&B==0){
    strcpy(msg,"The server chooses to betray.\nClient gets 3 years and Server is free.\n");
  }
  else if(A=='B'&&B==1){
    strcpy(msg, "The server chooses to be silent.\nClient is free and Server gets 3 years.\n");
  }
  else {
    strcpy(msg, "The server chooses to betray.\nClient gets 2 years and Server gets 2 years.\n");
  }
}

int main(int argc, char *argv[]){

  /*variables*/
  int status, clientsock, serversock, size;
  struct sockaddr_in server, dest;
  char buff[1024];
  int port;

  /*checks for proper user input*/
  if (argc != 2) {
    fprintf(stderr, "Usage: client hostname\n");
    exit(1);
  }
  if ((port = atoi(argv[1]))==0){perror("Failed port number\n"); exit(1);}

  /*create socket*/
  serversock = socket(AF_INET, SOCK_STREAM, 0);
  if(serversock<0){ perror("Failed to create socket");  exit(1);} /* checks for succeful socket connection*/
  memset(&server, 0, sizeof(server));
  memset(&dest, 0, sizeof(dest));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(port);

  /*call bind*/
  if(bind(serversock, (struct sockaddr *)&server, sizeof(server))){
    perror("bind error"); exit(1);
  }

  /*listen*/
  if( listen(serversock, 10)==-1){perror("Failed to listen/n"); exit(1);}

  /*Accept */
  while(1){
    size= sizeof(struct sockaddr_in);
    clientsock= accept(serversock, (struct sockaddr *)&dest, &size);
    if(clientsock==-1){perror("accept failed\n"); exit(1);}
    while(1){
      memset(buff, 0, sizeof(buff));
      if ((status = recv(clientsock, buff, sizeof(buff), 0))==-1){
        perror("Ending Connection"); exit(1);
      }
      else if(status == 0){ printf("Connection was closed\n"); return 0;}
      int serverAnswer = randomAnswer();
      sentence(buff[0], serverAnswer, buff);
      if ((send(clientsock, buff, strlen(buff), 0))==-1){
        perror("Failure sending message to client\n"); close(clientsock); break;
      }
    }
    close(clientsock);
  }
  close(serversock);
  return 0;
}
