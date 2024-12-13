#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int client_sock;

bool create_server_connect()
{
    int ret;
    struct sockaddr_in server_addr;

    // 创建客户端套接字
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Socket creation failed");
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // 将服务器的 IP 地址转换为网络字节序
    ret = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    if (ret <= 0) {
        perror("Invalid address or address not supported");
        close(client_sock);
        return false;
    }

    // 连接到服务器
    ret = connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("Connection failed");
        close(client_sock);
        return false;
    }

    return true;
}

bool send_data(const char *message)
{
    ssize_t sent_bytes;

    sent_bytes = send(client_sock, message, strlen(message), 0);
    if (sent_bytes < 0) {
        perror("Send failed");
        close(client_sock);
        return false;
    }

    return true;
}

bool recv_socket_data(char *buffer, int buff_len)
{
    ssize_t received_bytes;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // 接收客户端数据
    memset(buffer, '\0', buff_len);
    received_bytes = recv(client_sock, buffer, buff_len - 1, 0);
    if (received_bytes < 0) {
        perror("Receive failed");
        close(client_sock);
        return false;
    }

    buffer[received_bytes] = '\0';  // 防止数组越界

    return true;
}

int main()
{
    bool ret;
    char buffer[BUFFER_SIZE];
    const char *message = "Hello, Server!";

    ret = create_server_connect();
    if (!ret) {
        return -1;
    }
    std::cout << "Connected to server.\n";

    ret = send_data(message);
    if (!ret) {
        return -1;
    }
    std::cout << "Message sent to server: " << message << "\n";

    // 接收服务器响应
    ret = recv_socket_data(buffer ,sizeof(buffer));
    if (!ret) {
        return -1;
    }
    std::cout << "Received from server: " << buffer << std::endl;

    // 关闭客户端套接字
    close(client_sock);

    return 0;
}
