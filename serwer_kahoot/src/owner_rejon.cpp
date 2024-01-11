#include <iostream>
#include <vector>
#include <chrono>
#include <poll.h>

#include "../include/data_structurs.hpp"
#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;

#include "../include/send_data_rank.hpp"
#include "../include/question_to_json.hpp"


#include "../include/owner_rejon.hpp"



void handleStartWithOutRejoin(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue) // Handle start (owner)
{
    int gameID = gameData["gameID"].get<int>();
    int gamePin = gameData["gamePin"].get<int>();

    std::cout << gameID;
    std::cout << gamePin;

    json jsonMessage;
    jsonMessage["type"] = StartWaitingForPLayer;

    // Check game info
    if (!games->gamesList.empty() && gameID >= 0 && gameID < static_cast<int>(games->gamesList.size()) && games->gamesList[gameID].gamePin == gamePin && games->gamesList[gameID].gameStatus == "notWaitingForPlayers")
    {
        jsonMessage["status"] = SUCCESS;                         // Ssend success
        games->gamesList[gameID].gameStatus = WaitingForPlayers; // Set status
        games->gamesList[gameID].gameOwnerID = user.userID;        // Set game owner
        //games->gamesList[gameID].users.push_back(user);            // Push owner to users in game with empty nickname 
    }
    else if (!games->gamesList.empty() && (games->gamesList[gameID].gameStatus == WaitingForPlayers ||
                                           games->gamesList[gameID].gameStatus == STARTED)) // Game is waiting for players (lobby) or is started
    {
        jsonMessage["status"] = "waiting";
    }
    else
    {
        jsonMessage["status"] = "fail"; // Fail game is not existing
    }

    sendComunicate(user, jsonMessage, userList, messageQueue); // Send respond
}

void handleStartWithRejoin(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue) // Handle start with rejoin (owner)
{
    int gameID = gameData["gameID"].get<int>();
    int gamePin = gameData["gamePin"].get<int>();

    std::cout << gameID;
    std::cout << gamePin;

    json jsonMessage;
    jsonMessage["type"] = ReJoinOwner; // Prepare json type respond

    if (!games->gamesList.empty() && gameID >= 0 && gameID < static_cast<int>(games->gamesList.size()) && games->gamesList[gameID].gamePin == gamePin)
    {
        // for (User &userFind : games->gamesList[gameID].users) // Iterate through users in this game 
        // {

        //     if (userFind.userID == -1 && userFind.nickname == "") // Find owner
        //     {
        //         userFind.userID = user.userID; // Set owner
        //         break;
        //     }
        // }

        if (games->gamesList[gameID].gameStatus == STARTED) // If game is already on
        {
            json question = questionsToJson(&games->gamesList[gameID]);
            question["type"] = QuestionOwner;
            sendComunicate(user, question, userList, messageQueue); // Send current question to owner
        }
        else // Game is in lobby
        {
            json playersJson;
            for (User &players : games->gamesList[gameID].users) // Iterate through users in game
            {

                // if (players.userID != games->gamesList[gameID].gameOwnerID) // Check is user is no a owner
                // {
                    playersJson["type"] = PlayerJoin;
                    playersJson["player"] = players.nickname;

                    sendComunicate(user, playersJson, userList, messageQueue); // Send user nickname to owner, to let him see nicknames in lobby
                //}
            }
        }

        jsonMessage["status"] = games->gamesList[gameID].gameStatus; // Send success

        games->gamesList[gameID].gameOwnerID = user.userID; // Set game owner
    }
    else
    {
        jsonMessage["status"] = FAIL; // Game doesn't exists
    }

    sendComunicate(user, jsonMessage, userList, messageQueue); // Send communicate
}
