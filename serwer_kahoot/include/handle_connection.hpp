#ifndef HANDLE_CONNECTION_HPP
#define HANDLE_CONNECTION_HPP

void handleAccept(UserList *userList, User user);

void acceptClient(UserList *userList, int server_socket);

void disconnectClient(Games *games, UserList *userList, User user, int i);
#endif //HANDLE_CONNECTION_HPP
