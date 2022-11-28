// receives on packet via UDP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>


// provide the port value to receive on as command line argument
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval;
  struct addrinfo hints, *results, *ptr;

  // quick check if we provide the right arguments
  if (argc != 2) {
    std::cout << "Usage " << argv[0] << " receiving_port" << std::endl;
    return 1; // terminate
  }
  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we can use IPv6 or IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  
  if ((rval = getaddrinfo(NULL, argv[1], &hints, &results)) != 0) { // error
    std::cerr << "Error getting the local address and port: " << gai_strerror(rval) << std::endl;
    return 2;
  }

  // loop through the results from the linked list
  for (ptr = results; ptr != NULL; ptr = ptr->ai_next) {  
    if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) != -1) {
      break;  // socket successful, no need to search further
    }
  }

  // but we might reach the end of the list...
  if (ptr == NULL) {
    std::cerr << "Failed to open socket" << std::endl;
    return 3;
  }

  // we now need to bind the socket
  if (bind(sock, ptr->ai_addr, ptr->ai_addrlen) == -1) {
    std::cerr << "Cannot bind the socket" << std::endl;
    close(sock);
    return 4;
  }

  // to receive, define a buffer and a variable for the source address and length
  int byte_count;
  socklen_t fromlen;
  struct sockaddr_storage fromaddr;
  char buf[512];

  // initialize the address length parameter
  fromlen = sizeof(fromaddr);
  if ((byte_count = recvfrom(sock, buf, sizeof(buf)-1, 0, (sockaddr*)(&fromaddr), &fromlen)) < 0) {
    std::cerr << "Cannot receive packet" << std::endl;
    close(sock);
    return 5;
  }
  
  // we assume we receive a string, not a binary packet, so make it null string just in case
  buf[byte_count]=0;
  std::cout << buf << std::endl;

  if ((byte_count = sendto(sock, buf, byte_count, 0, (sockaddr*)(&fromaddr), fromlen)) < 0) {
    std::cerr << "Cannot send" << std::endl;
    close(sock);
    return 6;
  }
  
  close(sock);
}
