//DNS Query Program
//Author : Shivam Kumar
//Dated : 15/03/2018

//Header Files
#include <stdio.h>
#include <string.h>
#include <iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <vector>
#include <cstddef>
#include <algorithm>

using namespace std;

int asciiof(char c){
  return c;
}

char charofascii(int i){
  return i;
}

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

  while (true) {
    //getting hostname from user
    std::cout << "Enter a website name: " ;
    string hostname; cin >> hostname;
    //converting hostname to all lowercase
    std::transform(hostname.begin(), hostname.end(), hostname.begin(), ::tolower);
    if (hostname == "bye") {
      break;
    }

    //creating query

    //storing query prefix
    std::string query = "AA";
    char char_0 = charofascii(0);
    char char_1 = charofascii(1);
    query = query + char_1 + char_0 + char_0 + char_1 + char_0 + char_0 + char_0 + char_0 + char_0 + char_0;

    //storing hostname in query
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
          query = query + charofascii(counter[i]);
        }
        if (hostname[d]=='.') {
          d++;
        }
        query = query + hostname[d++];
      }
    }

    //storing suffix in query
    query = query + char_0 + char_0 + char_1 + char_0 + char_1;

    //converting query to valid type to send it to dns server
    unsigned char dns_query_message[query.length()];
    for (size_t i = 0; i < query.length(); i++) {
      dns_query_message[i] = query[i];
    }

    //sending query to server
    sendto(socket_descriptor, dns_query_message, query.length(), 0, (struct sockaddr*)&dns_server_socket_address, sizeof(dns_server_socket_address));

    socklen_t dns_server_socket_address_size= sizeof(dns_server_socket_address);

    //receiving and storing response
    unsigned char query_message[65536];
    recvfrom(socket_descriptor, (char *)query_message, 65536, 0, (struct sockaddr*)&dns_server_socket_address, & dns_server_socket_address_size);

    //retrieving and printing ip address from the response
    int a[4] ;
    int str_point = query.length() + 12;
    std::cout << "IP address of " << hostname << " is : ";
    for (size_t i = 0; i < 4; i++) {
      a[i] = query_message[str_point + i];
      if (i != 3) {
        std::cout << a[i] << '.';
      }
      else {
        std::cout << a[i] << '\n';
      }
    }
  }
}
