//DNS Query Program
//Author : Shivam Kumar
//Dated : 05/03/2018

//Header Files
#include <stdio.h>
#include <string.h>
#include <iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <vector>

using namespace std;

struct query_format{
  unsigned short id;
  unsigned char idk1:8;
  unsigned char isQuery:1;
  unsigned char idk2:7;
  unsigned short no_of_q;
  unsigned short no_of_a;
  unsigned short idk3;
  unsigned short idk4;
  unsigned short * question;
};

int asciiof(char c){
  return c;
}

char charofascii(int i){
  return i;
}

void save_q_to_query(query_format & query, string hostname) {
  query.question = new unsigned short[hostname.size() + 6];
  std::vector<int> counter(1,0);
  for (size_t i = 0; i < hostname.size(); i++) {
    if (hostname[i]=='.') {
      counter.push_back(0);
    }
    else {
      counter[counter.size() - 1]++;
    }
  }

  int c=0, d=0;

  for (size_t i = 0; i < counter.size(); i++) {
    for (size_t j = 0; j < counter[i]; j++) {
      if (j==0) {
        query.question[c++] = counter[i];
      }
      if (hostname[d]=='.') {
        d++;
      }
      query.question[c++] = asciiof(hostname[d++]);
    }
  }

  query.question[c++] = 0;
  query.question[c++] = 0;
  query.question[c++] = 1;
  query.question[c++] = 0;
  query.question[c++] = 1;

  for (size_t i = 0; i < hostname.size() + 6; i++) {
    std::cout << query.question[i] ;
  }
  std::cout << '\n';

}

struct response_format{
  unsigned short idk1[6];
  unsigned char ip_address[4];
};


int main(int argc, char * argv[]){

  //checking if dns server address is given or not
  if (argc < 2) {
    std::cout << "Please Enter DNS server address also and try again." << '\n';
    return 1;
  }

  //create socket descriptor
  int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

  //create dns server socket address
  struct sockaddr_in dns_server_socket_address;
  dns_server_socket_address.sin_family = AF_INET;
  dns_server_socket_address.sin_port = htons(53);
  dns_server_socket_address.sin_addr.s_addr = inet_addr(argv[1]);

  //getting hostname from user
  std::cout << "Enter a website name: " ;
  std::string hostname ;
  std::cin >> hostname;

  //creating query
  unsigned char query_message[65536];
  query_format * query_message_dns = (struct query_format *) & query_message[0];
  memset(query_message_dns, 0, 66536);
  query_message_dns -> id = htons(355);
  query_message_dns -> idk1 = 0;
  query_message_dns -> isQuery = 1;
  query_message_dns -> idk2 = 0;
  query_message_dns -> no_of_q = htons(1);
  query_message_dns -> no_of_a = 0;
  query_message_dns -> idk3 = 0;
  query_message_dns -> idk4 = 0;
  save_q_to_query(*query_message_dns, hostname);

  for (int i=0; i< 65536; i++){
  std::cout << query_message[i];}

  sendto(socket_descriptor, (char *)query_message, 65536, 0, (struct sockaddr*)&dns_server_socket_address, sizeof(dns_server_socket_address));

  socklen_t dns_server_socket_address_size= sizeof(dns_server_socket_address);

  recvfrom(socket_descriptor, (char *)query_message, 65536, 0, (struct sockaddr*)&dns_server_socket_address, & dns_server_socket_address_size);

  response_format * response = (struct response_format *) & query_message;
  for (size_t i = 0; i < 4; i++) {
    if (i != 4) {
      std::cout << asciiof(response->ip_address[i]) << "." ;
    }
    else std::cout << asciiof(response->ip_address[i]) << '\n';
  }

}
