#include <iostream>
#include <poll.h>

#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;


#include "../include/data_structurs.hpp"
#include "../include/send_data_rank.hpp"
#include "../include/check_if_user_in_game.hpp"
#include "../include/owner_rejon.hpp"


#include "../include/owner_start_lobby.hpp"


void startWatingForPlayer(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue) // Start lobby
{
    if (userNotInAnyGame(games, user)) // Check if user how want to start lobby is not in any other game 
    {
        int gameID = gameData["gameID"].get<int>();

        if (games->gamesList[gameID].gameOwnerID != -1) // Check if game hadn't had any previous owner
        {
            handleStartWithOutRejoin(games, gameData, user, userList, messageQueue); // Handle starting lobby without rejoining
            std::cout << " wlasciciela" << games->gamesList[gameID].gameOwnerID << std::endl;
        }
        else // Game had previous owner
            handleStartWithRejoin(games, gameData, user, userList, messageQueue); // Handle starting lobby with rejoining
    }
    else // User is in other game send communicate
    {
        json jsonMessage;
        jsonMessage["type"] = StartWaitingForPLayer;
        jsonMessage["status"] = UserAlreadyInGame;

        sendComunicate(user, jsonMessage, userList, messageQueue);
    }
}
