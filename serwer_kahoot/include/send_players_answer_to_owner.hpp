#ifndef SEND_PLAYERS_ANSWER_TO_OWNER_HPP
#define SEND_PLAYERS_ANSWER_TO_OWNER_HPP

void sendUserAnswerToOwner(Games *games, json gameData, User user, int gameID, UserList *userList, MessageQueue* messageQueue);

#endif // SEND_PLAYERS_ANSWER_TO_OWNER_HPP
