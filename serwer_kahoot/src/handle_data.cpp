#include <iostream>
#include <poll.h>
#include <vector>
#include <chrono>
#include <unistd.h>
#include "../include/data_structurs.hpp"

#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;

#include "../include/game_create.hpp"
#include "../include/start_game_send_player_to_lobby.hpp"
#include "../include/player_join_rejoin_game.hpp"
#include "../include/owner_start_lobby.hpp"
#include "../include/menage_points.hpp"
#include "../include/handle_connection.hpp"

#include "../include/handle_data.hpp"


void handleData(Games *games, json gameData, User user, UserList *userList, std::vector<GameDetails> *startedGamesList, MessageQueue* messageQueue) // Handle data from users
{
    if (gameData["type"].get<std::string>() == CreateGameStatus)
    {
        createGame(games, gameData, user, userList, messageQueue);
    }
    else if (gameData["type"].get<std::string>() == StartWaitingForPLayer)
    {
        startWatingForPlayer(games, gameData, user, userList, messageQueue);
    }
    else if (gameData["type"].get<std::string>() == JoinGameStatus)
    {
        joinGame(games, gameData, user, userList, messageQueue);
    }
    else if (gameData["type"].get<std::string>() == StartGameStatus)
    {
        startGame(games, gameData, user, userList, startedGamesList, messageQueue);
    }
    else if (gameData["type"].get<std::string>() == ANSWER)
    {
        putPointsToUser(games, gameData, user, userList, messageQueue);
    }
}

void readData(Games *games, UserList *userList, std::vector<GameDetails> *startedGamesList, MessageQueue* messageQueue)
{
    if (poll(userList->eventListener.data(), userList->eventListener.size(), 0))
    {

        char buf[256];
        ssize_t bytesRead;
        int timeout = 0;

        for (int i = 0; i < static_cast<int>(userList->eventListener.size()); i++)
        {
            User user = userList->users[i];
            std::string message;
            size_t newlinePos;

            int poll_users = poll(&userList->eventListener[i], 1, timeout);

            if (userList->buffer.size() < userList->eventListener.size())
                userList->buffer.resize(userList->eventListener.size());

            if (user.userID != -1 && poll_users > 0 && (userList->eventListener[i].revents & POLLIN))
            {
                try
                {
                    bytesRead = read(userList->eventListener[i].fd, buf, sizeof(buf) - 1);

                    if (bytesRead > 0)
                    {
                        buf[bytesRead] = '\0'; // Ensure null-termination

                        std::cout << buf << std::endl;

                        userList->buffer[i] += buf;

                        // Check if the newline character is present in the buffer
                        newlinePos = userList->buffer[i].find("\n");

                        if (newlinePos != std::string::npos)
                        {
                            message = userList->buffer[i].substr(0, newlinePos);

                            userList->buffer[i].erase(0, newlinePos + 1);

                            json jsonData;

                            printf("Received data: %s", message.c_str());

                            try
                            {
                                jsonData = json::parse(message);
                                printf("Type: %s", jsonData["type"].get<std::string>().c_str());

                                handleData(games, jsonData, user, userList, startedGamesList, messageQueue);

                                std::cout << user.userID << std::endl;
                            }
                            catch (...)
                            {
                                std::cerr << "Niezidentyfikowany wyjatek while parsing data" << std::endl;
                            }
                        }
                    }
                    else
                    {
                        disconnectClient(games, userList, user, i);
                    }
                }
                catch (...)
                {
                    std::cerr << "Niezidentyfikowany wyjatek podczas odczytu" << std::endl;
                }
            }
        }
    }
}
