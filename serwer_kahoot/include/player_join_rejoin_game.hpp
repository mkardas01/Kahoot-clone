#ifndef PLAYER_JOIN_REJOIN_GAME_HPP
#define PLAYER_JOIN_REJOIN_GAME_HPP
std::string handleAddingUserToGame(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue);

std::string handleReJoin(Games *games, json gameData, User user);

void joinGame(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue);
#endif // PLAYER_JOIN_REJOIN_GAME_HPP
