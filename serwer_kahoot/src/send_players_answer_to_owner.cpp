#include <iostream>
#include <poll.h>

#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;


#include "../include/data_structurs.hpp"
#include "../include/send_data_rank.hpp"


#include "../include/send_players_answer_to_owner.hpp"


void sendUserAnswerToOwner(Games *games, json gameData, User user, int gameID, UserList *userList, MessageQueue* messageQueue) // Send user answer to owner
{

    std::cout << "Found game with matching ID. answer to owner" << std::endl;

    // Iterate through users in game
    // for (const User &owner : games->gamesList[gameID].users)
    // {
        //cout << "Checking user in game with ID: " << owner.userID << endl;
        if (games->gamesList[gameID].gameOwnerID != -1) // Find owner
        {
            std::cout << "Found game owner with ID: " << games->gamesList[gameID].gameOwnerID << std::endl;

            json jsonMessage;
            jsonMessage["type"] = UserAnswer;
            jsonMessage["answerIndex"] = gameData["answer"]; // Prepare json message

            for (User &userGame : games->gamesList[gameID].users) // Find user in game
            {

                if (userGame.userID == user.userID)
                {
                    jsonMessage["nickname"] = userGame.nickname; // Set his nickname
                    break;
                }
            }

            sendComunicate(userList->users[games->gamesList[gameID].gameOwnerID], jsonMessage, userList, messageQueue); // Send nickname to owner

            std::cout << "Sending user ID " << user.userID << " to game owner with ID " << games->gamesList[gameID].gameOwnerID << std::endl;
            return;
        }
    //}
    std::cout << "Game owner not found in users list for the game." << std::endl;
}
