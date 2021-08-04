#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

    int rfork(string ipaddress)
    {
        pid_t pid = fork();
        if (pid > 0) //in parent
        {
            string id = to_string(pid);
            string string1 = "sudo criu dump -t ";
            string string2 = " -j -D checkpoint && echo OK";
            string string3 = string1 + id + string2;
            const char *c = string3.c_str();
            system(c);
            system("zip -r checkpoint.zip checkpoint");
            system("sshpass -p 05021999 scp main checkpoint.zip ahmedibrahim@192.168.80.159:/home/ahmed/Forking/");
            //creat client & connect to server
            
            int sock = 0, valread;
            struct sockaddr_in serv_addr;
            char *hello = "Hello from client";
            char buffer[1024] = {0};
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                printf("\n Socket creation error \n");
                return -1;
            }

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(8080);

            // Convert IPv4 and IPv6 addresses from text to binary form
            if (inet_pton(AF_INET, ipaddress.c_str(), &serv_addr.sin_addr) <= 0)
            {
                printf("\nInvalid address/ Address not supported \n");
                return -1;
            }

            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                printf("\nConnection Failed \n");
                return -1;
            }
            close(sock);
            return 0;
        }
        else //in child
        {
            sleep(10);
            return 1;
        }
    }
    
    string getmyip()

    {
        const char* google_dns_server = "8.8.8.8";
        int dns_port = 53;

        struct sockaddr_in serv;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        //Socket could not be created
        if(sock < 0)
        {
            std::cout << "Socket error" << std::endl;
        }

        memset(&serv, 0, sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = inet_addr(google_dns_server);
        serv.sin_port = htons(dns_port);

        int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
        if (err < 0)
        {
            std::cout << "Error number: " << errno
                << ". Error message: " << strerror(errno) << std::endl;
        }

        struct sockaddr_in name;
        socklen_t namelen = sizeof(name);
        err = getsockname(sock, (struct sockaddr*)&name, &namelen);

        char buffer[80];
        const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
        if(p != NULL)
        {
            //std::cout << "Local IP address is: " << buffer << std::endl;
        }

        else
        {
            std::cout << "Error number: " << errno
                << ". Error message: " << strerror(errno) << std::endl;
        }

        string s= buffer;
        close(sock);
        return s;
    }

    int main(){

        string ip_address = "192.168.80.159";
        int pid;
         if(ip_address==getmyip())
            pid=fork();
        else 
             pid= rfork("ip_address");

        if(pid == 0)
        {
            cout << "\nThis is the Parent node!\n";        

        }
        else
        {
            cout << "\nThis is the Child node!\n";

        }
        return 0;
    }