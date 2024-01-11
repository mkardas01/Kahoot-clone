#ifndef SEND_DATA_RANK_HPP
#define SEND_DATA_RANK_HPP
void sendComunicate(User user, json communicate, UserList *userList, MessageQueue* messageQueue);

void sendPointsSummary(Games *games, UserList *userList, int gameID, MessageQueue* messageQueue);
#endif // SEND_DATA_RANK_HPP
