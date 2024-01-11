#include <poll.h>
#include <netinet/in.h>

#include "../include/json.hpp"
#include "../include/const_data.hpp"
#include "../include/data_structurs.hpp"

using json = nlohmann::json;

#include "../include/send_data_rank.hpp"

void sendComunicate(User user, json communicate, UserList *userList, MessageQueue *messageQueue) // send message to specific user
{
    if (user.userID != -1) // Checj=k if user is not disconnected 
    {
        std::string message = communicate.dump() + "\n"; // Add end of line character at the end 
        const char *dataToSend = message.c_str();
        size_t dataSize = strlen(dataToSend);

        int poll_users = poll(&userList->sendListener[user.userID], 1, 0); 

        if (poll_users > 0 && (userList->sendListener[user.userID].revents & POLLOUT)) // Check if message can be send 
        {
            ssize_t sentBytes = send(userList->sendListener[user.userID].fd, dataToSend, dataSize, 0); // Send message 

            if (sentBytes == -1) // Check for errors
            {
                perror("Error sending data");
            }
        }else{
            // Add the message to the message queue
            messageQueue->messages.push_back(dataToSend);
            messageQueue->users.push_back(user);
            messageQueue->sendListener.push_back(userList->sendListener[user.userID]);
            messageQueue->startTime.push_back(std::chrono::steady_clock::now());
        }
    }
}

void sendPointsSummary(Games *games, UserList *userList, int gameID, MessageQueue* messageQueue) // Send points summary
{
    // Find the game with the given ID
    GameDetails &game = games->gamesList[gameID]; 

    // Sort users based on points in descending order
    std::sort(game.users.begin(), game.users.end(), [](const User &a, const User &b)
              { return a.points > b.points; });

    // Create a JSON message for points summary
    json jsonMessage;
    jsonMessage["type"] = PointsSummary;
    jsonMessage["users"] = json::array();

    // Iterate through sorted users and add them to the message
    for (const User &user : game.users)
    {
        // if (user.nickname != "") // Check if user is not a owner (owner can be showed in summary)
        // {
            json userObj;
            userObj["nickname"] = user.nickname;
            userObj["points"] = user.points;

            jsonMessage["users"].push_back(userObj); // Add users
        //}
    }

    // Send the points summary to each player in game
    for (const User &gameUser : game.users)
    {
        sendComunicate(gameUser, jsonMessage, userList, messageQueue);
    }

    // Send it to game owner
    sendComunicate(userList->users[games->gamesList[gameID].gameOwnerID], jsonMessage, userList, messageQueue);
}
