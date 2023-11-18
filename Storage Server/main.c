#include "Utils/client_handler.h"

int main()
{
    // SS_NM_PORT, SS_Client_PORT;
    SS_NM_PORT = findAvailablePort();
    SS_Client_PORT = findAvailablePort();

    if (SS_NM_PORT == -1 || SS_Client_PORT == -1)
    {
        printf("No available port found. Exiting.\n");
        return -1;
    }

    printf("\nStorage Server Port for communication with naming server: %d\n", SS_NM_PORT);
    printf("Storage Server Port for communication with client: %d\n\n", SS_Client_PORT);

    sendInfoToNM();

    int SS_client_fd, SS_NM_fd, temp_fd;
    struct sockaddr_in SS_client_address, SS_NM_address;
    struct sockaddr_in serv_addr;

    SS_client_address.sin_family = AF_INET;
    SS_client_address.sin_addr.s_addr = INADDR_ANY;
    SS_client_address.sin_port = htons(SS_Client_PORT);

    SS_NM_address.sin_family = AF_INET;
    SS_NM_address.sin_addr.s_addr = INADDR_ANY;
    SS_NM_address.sin_port = htons(SS_NM_PORT);

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // serv_addr.sin_port = htons(SS_NM_PORT);

    SS_NM_fd = createServerSocket();
    SS_client_fd = createServerSocket();

    // bindServerSocket(SS_NM_fd, &serv_addr);

    // char server_ip[INET_ADDRSTRLEN]; // Buffer for the IP address

    // startListening(SS_client_fd);
    // startListening(SS_NM_fd);
    // SS_NM_fd = createServerSocket();
    bindServerSocket(SS_NM_fd, &SS_NM_address);
    bindServerSocket(SS_client_fd, &SS_client_address);

    pthread_t client_thread, nm_thread;

    // print the SS_client_fd
    // printf("SS_client_fd : %d \n", SS_client_fd);
    startListening(SS_client_fd);

    // printf("billu bageele\n");
    // printf("SS_NM_fd : %d \n", SS_NM_fd);
    startListening(SS_NM_fd);

    // SS_NM_fd = createServerSocket();

    // bindServerSocket(SS_NM_fd, &SS_NM_address);
    pthread_create(&client_thread, NULL, listenForClients, (void *)&SS_client_fd);
    pthread_create(&nm_thread, NULL, listenFromNameServer, (void *)&SS_NM_fd);

    pthread_join(client_thread, NULL);
    pthread_join(nm_thread, NULL);

    // while(1) {
    // char message[1024];
    // printf(CYN"Enter Message: "reset);
    // fgets(message, 1024, stdin);
    // message[strcspn(message, "\n")] = 0; // Remove newline character if present
    // sendMessage(sock, message);
    // readMessage(sock); // Read confirmation message from server
    // }

    close(SS_client_fd);
    close(SS_NM_fd);
    return 0;
}
