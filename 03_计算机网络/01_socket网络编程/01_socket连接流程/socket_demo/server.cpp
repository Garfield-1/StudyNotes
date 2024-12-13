#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int server_sock, client_sock;

bool create_socket_listen()
{
    struct sockaddr_in server_addr;
    int ret = 0;

    // 创建服务器套接字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return false;
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字到指定端口
    ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("Bind failed");
        close(server_sock);
        return false;
    }

    // 进入监听状态
    ret = listen(server_sock, 5);
    if (ret < 0) {
        perror("Listen failed");
        close(server_sock);
        return false;
    }

    return true;
}

bool recv_socket_data(int server_sock, char *buffer, int buff_len)
{
    ssize_t received_bytes;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // 接受客户端连接
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock < 0) {
        perror("Accept failed");
        close(server_sock);
        return false;
    }

    // 接收客户端数据
    memset(buffer, '\0', buff_len);
    received_bytes = recv(client_sock, buffer, buff_len - 1, 0);
    if (received_bytes < 0) {
        perror("Receive failed");
        close(client_sock);
        close(server_sock);
        return false;
    }

    buffer[received_bytes] = '\0';  // 防止数组越界

    return true;
}

bool send_respond(const char *response)
{
    ssize_t buff_len;

    buff_len = send(client_sock, response, strlen(response), 0); // 发送响应数据
    if (buff_len == -1) {
        return false;
    }

    return true;
}

int main()
{
    bool ret;
    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello, Client!";

    std::cout << "Server start" << std::endl;
    ret = create_socket_listen();
    if (!ret) {
        return -1;
    }
    std::cout << "Server listening on port " << PORT << "...\n";

    ret = recv_socket_data(server_sock, buffer, sizeof(buffer));
    if (!ret) {
        return -1;
    }
    std::cout << "Received from client: " << buffer << std::endl;

    ret = send_respond(response);
    if (!ret) {
        return -1;
    }

    close(server_sock);
    close(client_sock);

    return 0;
}
