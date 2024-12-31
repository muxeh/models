#ifndef SOCKET_UDP_HPP
#define SOCKET_UDP_HPP

/**
 * \file 
 * \author muxeh
 * \brief Class for sending and receiving messages over UDP
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class SocketUDP {

    public:
        SocketUDP();
        ~SocketUDP();

    private:
        /// Constants
        static constexpr uint16_t MAX_ADDR_SIZE = 100;
        static constexpr uint16_t MAX_BUFFER_SIZE = 1024;

        /// Functions
        // Configure sending socket
        void configSend(const char* ip, const uint16_t port);
        // Configure receiving socket
        void configRecv(const uint16_t port);
        // Send message over UDP
        void send(const char* buffer, const uint16_t size);
        // Recieve message sent over UDP if available
        void recv(char* buffer, const uint16_t size);

        /// Members
        // Flag for send configuration status (true = configured)
        bool m_send_confgd;
        // Send IP address
        char m_send_ip[MAX_ADDR_SIZE];
        // Send port
        uint16_t m_send_port;
        // Send socket
        int m_send_socket;
        // Send server address
        struct sockaddr_in m_send_server_addr;
        // Flag for receive configuration status (true = configured)
        bool m_recv_confgd;
        // Receive port
        uint16_t m_recv_port;
        // Receive socket
        int m_recv_socket;

};

#endif /* SOCKET_UDP_HPP */
