#include <iostream>
#include <poll.h>

#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;


#include "../include/data_structurs.hpp"
#include "../include/send_data_rank.hpp"
#include "../include/question_to_json.hpp"


#include "../include/send_questions_or_endofgame.hpp"

void sendQuestionsOrEndOfGame(Games *games, int gameID, bool endOfGame, UserList *userList, MessageQueue* messageQueue) // Handle questions and end of game
{

    json jsonMessage;
    GameDetails game = games->gamesList[gameID];

    std::cout << "Found game with matching ID. questions" << std::endl;

    for (const User &gameUser : game.users) // Send current question or end of game to all users 
    {
        if (!endOfGame) // If game is still on
        {
            jsonMessage = questionsToJson(&game); // Get question
            sendComunicate(gameUser, jsonMessage, userList, messageQueue); // Send it 
        }
        else // If game is not still on
        {
            jsonMessage["type"] = EndOfGame; 
            sendComunicate(gameUser, jsonMessage, userList, messageQueue); // Send comunicate about ending of game
        }
    }

    //Handle owner
    if (!endOfGame) // If game is still on
        {
            jsonMessage = questionsToJson(&game); // Get question
            jsonMessage["type"] = QuestionOwner;
            
            sendComunicate(userList->users[games->gamesList[gameID].gameOwnerID], jsonMessage, userList, messageQueue); // Send it 
        }
        else // If game is not still on
        {
            jsonMessage["type"] = EndOfGame; 
            sendComunicate(userList->users[games->gamesList[gameID].gameOwnerID], jsonMessage, userList, messageQueue);// Send comunicate about ending of game
        }

    if(!endOfGame)
        games->gamesList[gameID].startTime = std::chrono::time_point_cast<std::chrono::steady_clock::duration>(std::chrono::steady_clock::now()); // Start timer of current question

}
