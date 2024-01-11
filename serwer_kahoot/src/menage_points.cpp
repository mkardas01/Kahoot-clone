#include <iostream>
#include <poll.h>
#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;

#include "../include/data_structurs.hpp"
#include "../include/send_players_answer_to_owner.hpp"
#include "../include/menage_points.hpp"

void putPointsToUser(Games *games, json gameData, User user, UserList *userList, MessageQueue *messageQueue) // Add points to user
{

    int gameID = gameData["gameID"].get<int>();

    if (games->gamesList[gameID].questions[games->gamesList[gameID].currentQuestion].indexOfCorrectAnswer == gameData["answer"]) // Check if answer is correct
    {

        auto currentTime = std::chrono::steady_clock::now();
        auto points = std::chrono::duration_cast<std::chrono::seconds>(currentTime - games->gamesList[gameID].startTime); // Calculate tiem

        // Update the user in the gamesList
        int position = 0;
        for (auto u : games->gamesList[gameID].users) // Iterate through users
        {
            if (u.userID == user.userID)
            {
                games->gamesList[gameID].users[position].points += 30 - points.count(); // Add points
                
                break;
            }

            position++;
        }
    }
    
    games->gamesList[gameID].usersWhoAnswered++;

    sendUserAnswerToOwner(games, gameData, user, gameID, userList, messageQueue); // Send owner info about user's answer
}
