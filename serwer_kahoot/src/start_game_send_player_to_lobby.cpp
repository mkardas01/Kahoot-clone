#include <iostream>
#include <poll.h>

#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;


#include "../include/data_structurs.hpp"
#include "../include/send_data_rank.hpp"
#include "../include/send_questions_or_endofgame.hpp"


#include "../include/start_game_send_player_to_lobby.hpp"

void sendPlayerToWaitingList(Games *games, User user, int gameID, UserList *userList, MessageQueue* messageQueue) // handle lobby 
{
    std::cout << "wysylam gracza";
    GameDetails game = games->gamesList[gameID];

    std::cout << "Found game with matching ID. waiting list" << std::endl;

    // Iterate through game's users
    // for (const User &gameUser : game.users)
    // {
    //     cout << "Checking user in game with ID: " << gameUser.userID << endl;
    //     if (gameUser.userID == game.gameOwnerID && user.userID != game.gameOwnerID) // Find owner
    //     {
            std::cout << "Found game owner with ID: " << game.gameOwnerID << std::endl;

            json jsonMessage;
            jsonMessage["type"] = PlayerJoin;
            jsonMessage["player"] = user.nickname;

            sendComunicate(userList->users[games->gamesList[gameID].gameOwnerID], jsonMessage, userList, messageQueue); // Send info about joining user to game owner

            std::cout << "Sending user ID " << user.userID << " to game owner with ID " << game.gameOwnerID << std::endl;

            return;
    //     }
    // }
    std::cout << "Game owner not found in users list for the game." << std::endl;

    std::cout << "Game not found for user with ID " << user.userID << std::endl;
}

void startGame(Games *games, json gameData, User user, UserList *userList, std::vector<GameDetails>* startedGamesList, MessageQueue* messageQueue) // start game (stop waiting in lobby for players)
{
    json jsonMessage;
    jsonMessage["type"] = StartGameStatus; // Prepare json message 

    int gameID = gameData["gameID"].get<int>();

    jsonMessage["status"] = "started";

    games->gamesList[gameID].gameStatus = STARTED; // Change game status
  
    startedGamesList->push_back(games->gamesList[gameID]);
    
    sendComunicate(user, jsonMessage, userList, messageQueue); // Send respond to owner
    sendQuestionsOrEndOfGame(games, gameID, false, userList, messageQueue); // Send questions to players
}
