#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>


// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval;
  struct addrinfo hints, *results, *ptr;

  // quick check if we preovide the right arguments
  if (argc != 3) {
    std::cout << "Usage " << argv[0] << " destination_host destination_port" << std::endl;
    return 1; // terminate
  }
  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo(argv[1], argv[2], &hints, &results)) != 0) { // error
    std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
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

  // if not, we are ready to send here
  // add the code to send "hello world" here

  std::string msg = "Hello world!";
  int chsent = sendto(sock, msg.c_str(), msg.size()+1, 0, ptr->ai_addr, ptr->ai_addrlen);
  if (chsent != msg.size()+1) {
    std::cerr << "Failed to send. " << chsent << " chars sent!" << std::endl;
  }

  std::cout << "Receiving now..." << std::endl;

  int byte_count;
  socklen_t fromlen;
  struct sockaddr_storage fromaddr;
  char buf[512];

  // initialize the length of the address
  fromlen = sizeof(fromaddr);
  if ((byte_count = recvfrom(sock, buf, sizeof(buf)-1, 0, (sockaddr*)(&fromaddr), &fromlen)) < 0) {
    std::cerr << "Cannot receive." << std::endl;
    close(sock);
    return 4;
  }
  
  // we assume we receive a string, not a binary packet, so make it null string just in case
  buf[byte_count]=0;
  std::cout << buf << std::endl;
  
  close(sock);
}
