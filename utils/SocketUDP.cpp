#include "SocketUDP.hpp"

SocketUDP::SocketUDP() : m_send_confgd(false),
                         m_recv_confgd(false),
                         m_send_port(0),
                         m_recv_port(0),
                         m_send_socket(0),
                         m_recv_socket(0),
                         m_send_ip(""),
                         m_send_server_addr({}) {
};

SocketUDP::~SocketUDP() {
    // Check if send socket is configured
    if (m_send_confgd) {
        // Close socket
        close(m_send_socket);
    }
    // Check if receive socket is configured
    if (m_recv_confgd) {
        // Close socket
        close(m_recv_socket);
    }
};

void SocketUDP::configSend(const char* ip, const uint16_t port) {
    // Create a UDP socket for sending
    m_send_socket = socket(AF_INET, SOCK_DGRAM, 0);
    // Check if socket creation failed
    if (m_send_socket < 0) {
        // Report error
        perror("Sender socket creation failed");
        // Exit with error status
        std::exit(1);
    }
    // Configure server address
    struct sockaddr_in m_send_server_addr = {};
    std::memset(&m_send_server_addr, 0, sizeof(m_send_server_addr));
    m_send_server_addr.sin_family = AF_INET;
    m_send_server_addr.sin_port = htons(m_send_port);
    // Check if server address configuration failed
    if (inet_pton(AF_INET, m_send_ip, &m_send_server_addr.sin_addr) <= 0) {
        // Report error
        perror("Invalid IP address");
        // Close socket
        close(m_send_socket);
        // Exit with error status
        std::exit(1);
    }
    // Set send configure status to true
    m_send_confgd = true;
};

void SocketUDP::configRecv(const uint16_t port) {
    // Create a UDP socket for receiving
    int m_recv_socket = socket(AF_INET, SOCK_DGRAM, 0);
    // Check if socket creation failed
    if (m_recv_socket < 0) {
        // Report error
        perror("Receiver socket creation failed");
        // Exit with error status
        std::exit(1);
    }
    // Configure server address
    struct sockaddr_in receiver_addr = {};
    std::memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(m_recv_port);
    receiver_addr.sin_addr.s_addr = INADDR_ANY;
    // Bind socket and check if it failed
    if (bind(m_recv_socket, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) < 0) {
        // Report error
        perror("Bind failed");
        // Close socket
        close(m_recv_socket);
        // Exit with error status
        std::exit(1);
    }
    // Set receive configure status to true
    m_recv_confgd = true;
};

void SocketUDP::send(const char* buffer, const uint16_t size) {
    // Check if send is configured
    if (!m_send_confgd) {
        // Report error
        std::cerr << "Send socket not configured, aborting send call.\n";
        // Exit function
        return;
    }
    // Send data buffer
    ssize_t bytes_sent = sendto(m_send_socket, buffer, size, 0,
                               (struct sockaddr*)&m_send_server_addr, sizeof(m_send_server_addr));
    // Check if zero bytes were sent
    if (bytes_sent < 0) {
        // Report error
        perror("Send failed");
        // Close socket
        close(m_send_socket);
        // Exit with error status
        std::exit(1);
    }
};

void SocketUDP::recv(char* buffer, const uint16_t size) {
    // Temp vars
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    ssize_t bytes_received = recvfrom(m_recv_socket, buffer, size, 0,
                                     (struct sockaddr*)&sender_addr, &sender_addr_len);

    if (bytes_received < 0 &&
        errno != EAGAIN &&
        errno != EWOULDBLOCK) {
        // Report error
        perror("Failed to receive data");
        // Exit with error status
        std::exit(1);
    }
};
