#ifndef OWNER_REJON_HPP
#define OWNER_REJON_HPP

void handleStartWithOutRejoin(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue);

void handleStartWithRejoin(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue);

#endif // OWNER_REJON_HPP
