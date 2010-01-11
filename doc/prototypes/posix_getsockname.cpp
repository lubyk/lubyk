#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> // inet_ntoa

void print_port(int sockd, struct sockaddr_in &addr) {
  socklen_t addrlen;
  /* We must put the length in a variable.              */
  addrlen = sizeof(addr);
  /* Ask getsockname to fill in this socket's local     */
  /* address.                                           */
  if (getsockname(sockd, (struct sockaddr *)&addr, &addrlen) == -1) {
    perror("getsockname() failed");
    return;
  }

  /* Print it. The IP address is often zero beacuase    */
  /* sockets are seldom bound to a specific local       */
  /* interface.                                         */
  printf("Local IP address is: %s\n", inet_ntoa(addr.sin_addr));
  printf("Local port is: %d\n", (int) ntohs(addr.sin_port));
}

int main(int argc, char* argv[])
{
  int sockd, sockd2;
  socklen_t addrlen;
  struct sockaddr_in my_name, peer_name;
  int status;

  /* create a socket */
  sockd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockd == -1)
  {
    perror("Socket creation error");
    exit(1);
  }

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s port_number\n", argv[0]);
    exit(1);
  }

  /* server address  */
  my_name.sin_family = AF_INET;
  my_name.sin_addr.s_addr = INADDR_ANY;
  my_name.sin_port = htons(atoi(argv[1]));

  status = bind(sockd, (struct sockaddr*)&my_name, sizeof(my_name));
  if (status == -1)
  {
    perror("Binding error");
    exit(1);
  }

  // find port

  print_port(sockd, my_name);


  status = listen(sockd, 5);
  if (status == -1)
  {
    perror("Listening error");
    exit(1);
  }

  for(;;)
  {
    /* wait for a connection */
    addrlen = sizeof(peer_name);
    sockd2 = accept(sockd, (struct sockaddr*)&peer_name, &addrlen);
    if (sockd2 == -1)
    {
      perror("Wrong connection");
      exit(1);
    }
    write(sockd2, "Hello!\n", 7);
    close(sockd2);
  }
  return 0;
}