#ifndef START_GAME_SEND_PLAYER_TO_LOBBY_HPP
#define START_GAME_SEND_PLAYER_TO_LOBBY_HPP
void sendPlayerToWaitingList(Games *games, User user, int gameID, UserList *userList, MessageQueue* messageQueue);

void startGame(Games *games, json gameData, User user, UserList *userList, std::vector<GameDetails>* startedGamesList, MessageQueue* messageQueue);

#endif // START_GAME_SEND_PLAYER_TO_LOBBY_HPP
