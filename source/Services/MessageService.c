/**
 * @authors Stanislav Stoianov,
 */

#include <pthread.h>
#include "MessageService.h"
#include "TimeService.h"
#include "UserEntety.h"
#include "stdbool.h"

pthread_mutex_t mutex;

void *sendThread(void *param);

void *recvThread(void *param);


/**
 * TODO FIXME forward username to variable
 */

//char *username = setName();

/**
 * FIXME IMPORTANT test functions
 * @param clientSocket
 */

void *messageHandler(SOCKET *clientSocket) {
    pthread_t send_thread_id;
    pthread_t recv_thread_id;

    pthread_create(&send_thread_id, NULL, sendThread, (void *) &clientSocket);
    pthread_detach(send_thread_id);

    pthread_create(&recv_thread_id, NULL, recvThread, (void *) &clientSocket);
    pthread_join(recv_thread_id, NULL);

    pthread_exit(NULL);

    return (void *) 2;
}

void *sendThread(void *param) {
    SOCKET clientSocket = (SOCKET) param;
    char message[1024];
    char text[992];
    while (true) {
        scanf("%s", text);
        sprintf(message, "%s: %s", User.name, text);
        if (send(clientSocket, message, 1024, 0) == SOCKET_ERROR) {
            printf("[%s] ERROR: Can't send a message, connection will be closing\n",
                   getCurrentTime());
            return (void *) 2;
        }
    }
}

void *recvThread(void *param) {
    SOCKET clientSocket = (SOCKET) param;
    char receive[1024];
    while (true) {
        if (recv(clientSocket, receive, 1024, 0)) {
            printf("[%s] ERROR: Can't receive a message, connection will be closing\n",
                   getCurrentTime());
            return (void *) 2;
        }
        pthread_mutex_lock(&mutex);
        printf("%s", receive);
        pthread_mutex_unlock(&mutex);
    }
}