#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char message[100];

    int id, priority, exec_time;

    printf("Enter Task ID: ");
    scanf("%d", &id);

    printf("Enter Priority: ");
    scanf("%d", &priority);

    printf("Enter Execution Time (seconds): ");
    scanf("%d", &exec_time);

    sprintf(message, "%d|%d|%d", id, priority, exec_time);

    send(sock, message, strlen(message), 0);

    printf("Task Sent to Server!\n");

    close(sock);

    return 0;
}