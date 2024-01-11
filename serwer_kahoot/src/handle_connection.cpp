#include <poll.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/data_structurs.hpp"
#include "../include/const_data.hpp"
#include "../include/handle_connection.hpp"

void handleAccept(UserList *userList, User user) // Handle accepting new connection
{
    // Find the first available userID
    int firstAvailableUserID = 0;

    for (const auto &existingUser : userList->users)
    {
        if (existingUser.userID != -1) // userID = -1 means user is disconnected
        {
            firstAvailableUserID++;
        }
        else
        {
            break;
        }
    }

    // Create pollfd

    user.userID = firstAvailableUserID;

    pollfd event;
    event.fd = user.client_socket;
    event.events = POLLIN;

    if (static_cast<int>(userList->eventListener.size()) <= firstAvailableUserID)
        userList->eventListener.resize(userList->eventListener.size() + 1); // Resize eventListener vector if needed

    userList->eventListener[user.userID] = event;

    event.events = POLLOUT;

    if (static_cast<int>(userList->sendListener.size()) <= firstAvailableUserID)
        userList->sendListener.resize(userList->sendListener.size() + 1); // Resize sendListener vector if needed

    userList->sendListener[user.userID] = event;

    if (static_cast<int>(userList->users.size()) <= firstAvailableUserID)
        userList->users.resize(userList->users.size() + 1); // Resize users vector if needed

    userList->users[user.userID] = user; // Add user to userList

    printf("Assigned userID: %d", user.userID);
}
void acceptClient(UserList *userList, int server_socket) // Accept new connection
{
    struct sockaddr_in client_addr;
    socklen_t addr_size;
    User user;

    addr_size = sizeof(client_addr);
    user.client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size); // Accept connection

    if (user.client_socket == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // No incoming connection, try again later
            return;
        }
        else
        {
            perror("Error accepting connection");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
    }

    if (fcntl(user.client_socket, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("Error setting nonblocking mode in new connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from %s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    handleAccept(userList, user); // Handle accept
}

void disconnectClient(Games *games, UserList *userList, User user, int i) // Disconnect user
{
    // Zamknięcie połączenia przez klienta
    std::cout << "Connection closed by client " << userList->users[i].userID << std::endl;

    close(userList->users[i].client_socket); // Close any exitisng descriptor
    close(userList->eventListener[i].fd);
    close(userList->sendListener[i].fd);
    close(user.client_socket);

    bool found = false;

    for (int p = 0; p < static_cast<int>(games->gamesList.size()); p++) // Find disconnected user in game
    {
        GameDetails game = games->gamesList[p];
        std::cout << "sprawdzam gre o id " << p << std::endl;

        if (game.gameOwnerID == user.userID)
        {
            games->gamesList[p].gameOwnerID = -1;
            std::cout << "usuwam wlasciciela " << game.gameOwnerID << std::endl;
            break;
        }

        for (int u = 0; u < static_cast<int>(game.users.size()); u++) // Find user in game
        {
            std::cout << "sprawdzam uzytkownika o id " << u << std::endl;

            if (game.users[u].userID == user.userID) // Set his userid to -1 (id -1 means disconnected)
            {
                games->gamesList[p].users[u].userID = -1;

                // if (game.users[u].userID == game.gameOwnerID) // Check if user was a gameowner if yes set gameownerid to -1
                //     games->gamesList[p].gameOwnerID = -1;

                found = true;
                break;
            }
        }

        if (found)
        {
            break;
        }
    }

    if (userList->users[i].userID == userList->users[userList->users.size() - 1].userID) // If user in userlist was at the end of userlist vector, delete it
    {
        userList->buffer.erase(userList->buffer.begin() + i);
        userList->eventListener.erase(userList->eventListener.begin() + i);
        userList->sendListener.erase(userList->sendListener.begin() + i);
        userList->users.erase(userList->users.begin() + i);
    }
    else // Otherwise put empoty user with id -1
    {

        User emptyUser;
        emptyUser.userID = -1;

        // Inserting emptyUser into userList
        userList->users[i] = emptyUser;

        // Inserting an empty string into the buffer
        userList->buffer[i] = "";
    }
}
