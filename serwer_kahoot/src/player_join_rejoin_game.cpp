#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <poll.h>

#include "../include/data_structurs.hpp"
#include "../include/const_data.hpp"
#include "../include/json.hpp"
using json = nlohmann::json;

#include "../include/send_data_rank.hpp"
#include "../include/start_game_send_player_to_lobby.hpp"
#include "../include/check_if_user_in_game.hpp"
#include "../include/check_nickname_availability.hpp"

#include "../include/player_join_rejoin_game.hpp"


std::string handleAddingUserToGame(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue)
{

    std::string status;

    int gameID = gameData["gameID"].get<int>();
    int gamePin = gameData["gamePin"].get<int>();
    std::string gameNickname = gameData["nickname"].get<std::string>();

    if (!games->gamesList.empty() && gameID >= 0 &&
        gameID < static_cast<int>(games->gamesList.size()) &&
        games->gamesList[gameID].gamePin == gamePin &&
        games->gamesList[gameID].gameStatus == WaitingForPlayers) // Check if game is available 

    {

        status = "success";
        user.nickname = gameNickname;
        games->gamesList[gameID].users.push_back(user); // Add users to game 

        sendPlayerToWaitingList(games, user, gameID, userList, messageQueue); // Send user to lobby (owner see who joined the game)
    }
    else if (!games->gamesList.empty() && games->gamesList[gameID].gameStatus == STARTED)
    {
        status = STARTED;
    }
    else
    {
        status = FAIL;
    }

    return status;
}

std::string handleReJoin(Games *games, json gameData, User user) // handle rejoin to game (only players)
{
    int gameID = gameData["gameID"].get<int>();
    std::string gameNickname = gameData["nickname"].get<std::string>();

    for (User &userFind : games->gamesList[gameID].users) // Iterate through games 
    {
        if (userFind.nickname == gameNickname) // Find available nickname to rejoin 
        {
            userFind.userID = user.userID;
            break;
        }
    }

    return "reJoin";
}

void joinGame(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue) // Join game (only players)
{
    int gameID = gameData["gameID"].get<int>();
    int gamePin = gameData["gamePin"].get<int>();
    std::string gameNickname = gameData["nickname"].get<std::string>();

    std::cout << gameID << std::endl;
    std::cout << gamePin << std::endl;

    json jsonMessage;
    jsonMessage["type"] = "joinGame";

    if (userNotInAnyGame(games, user)) // Check if user is not in any other game 
    {

        std::string checkNickNameStatus = nickNameStatus(games, gameID, gameNickname); // Check nickname status

        if (checkNickNameStatus == "unavailable")
        {
            //Nickname not available
            jsonMessage["status"] = "nickNameNotAvailable";
        }
        else if (checkNickNameStatus == "rejoin")
        {
            // Can rejoin to this nickname
            jsonMessage["status"] = handleReJoin(games, gameData, user);
        }
        else if (checkNickNameStatus == "available")
        {
            // Nickname available
            jsonMessage["status"] = handleAddingUserToGame(games, gameData, user, userList, messageQueue);
        }
    }
    else
    {
        jsonMessage["status"] = "userAlreadyInGame";
    }

    sendComunicate(user, jsonMessage, userList, messageQueue); // Send message to user
}
