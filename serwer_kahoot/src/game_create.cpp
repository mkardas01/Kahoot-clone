#include <iostream>
#include <string>
#include <poll.h>
#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;

#include "../include/data_structurs.hpp"
#include "../include/send_data_rank.hpp"



#include "../include/game_create.hpp"

void createGame(Games *games, json gameData, User user, UserList *userList, MessageQueue* messageQueue)
{

    GameDetails newGame; // Create new game

    newGame.gameID = games->gamesList.size();                         // Set gameid
    newGame.gamePin = rand() % 9000 + 1000;                           // Generate gamepin
    newGame.questionsNumber = gameData["questionsNumber"].get<int>(); // Set question number

    for (const auto &questionData : gameData["questions"])
    {
        Question question;
        question.question = questionData["question"].get<std::string>();

        for (int i = 0; i < 4; ++i)
        {
            question.answers[i] = questionData["answers"][i].get<std::string>();
        }

        question.indexOfCorrectAnswer = (int)questionData["indexOfCorrectAnswer"].get<int>();
        newGame.questions.push_back(question); // Set questions
    }

    std::cout << "Created game with ID: " << newGame.gameID << std::endl;

    // Add new game to gameList
    games->gamesList.push_back(newGame);

    json jsonMessage;
    jsonMessage["type"] = CreateGameStatus;
    jsonMessage["status"] = SUCCESS;
    jsonMessage["gameID"] = newGame.gameID;
    jsonMessage["gamePin"] = newGame.gamePin; // Create json respond 

    sendComunicate(user, jsonMessage, userList, messageQueue); // Send json respond to the creator 
}
