#include <iostream>
#include <vector>
#include <poll.h>

#include "../include/json.hpp"
#include "../include/const_data.hpp"
using json = nlohmann::json;

#include "../include/data_structurs.hpp"
#include "../include/const_data.hpp"

#include "../include/send_data_rank.hpp"
#include "../include/send_questions_or_endofgame.hpp"

#include "../include/check_if_next_question.hpp"

void checkIfSendNextQuestion(Games *games, UserList *userList, std::vector<GameDetails> *startedGamesList, MessageQueue *messageQueue) // Check if it's time to send new question to players
{

    for (const GameDetails &checkGame : *startedGamesList)
    {
        GameDetails &game = games->gamesList[checkGame.gameID];

        auto currentTime = std::chrono::steady_clock::now();
        if (games->gamesList[game.gameID].currentQuestion < game.questionsNumber)
        {
            if ((currentTime - game.startTime) >= std::chrono::seconds(TIME_TO_RESPOND + 10) 
                || games->gamesList[game.gameID].usersWhoAnswered == static_cast<int>(games->gamesList[game.gameID].users.size()))
            {
                games->gamesList[game.gameID].usersWhoAnswered = 0;
                games->gamesList[game.gameID].currentQuestion++;
                sendPointsSummary(games, userList, game.gameID, messageQueue);               // Send rank
                sendQuestionsOrEndOfGame(games, game.gameID, false, userList, messageQueue); // Send questions
            }
        }
        else
        {
            endOfGame(games, userList, startedGamesList, game, messageQueue);
        }
    }
}

void endOfGame(Games *games, UserList *userList, std::vector<GameDetails> *startedGamesList, GameDetails game, MessageQueue *messageQueue)
{
    // stop game if no more questions

    std::cout << "usuwam graczy z gry";

    sendPointsSummary(games, userList, game.gameID, messageQueue);              // Send summary to everyone
    sendQuestionsOrEndOfGame(games, game.gameID, true, userList, messageQueue); // Send end of game to everoyne

    games->gamesList[game.gameID].gameStatus = NotWaitingForPlayers; // Change status, game is available to start
    games->gamesList[game.gameID].currentQuestion = 0;               // Reset currentquestion counter
    games->gamesList[game.gameID].gameOwnerID = -2;                  // Game is waiting for owner
    games->gamesList[game.gameID].users.clear();                     // Delete users from game
    auto it = std::find_if(startedGamesList->begin(), startedGamesList->end(),
                           [&game](const GameDetails &g)
                           {
                               return g.gameID == game.gameID;
                           });

    if (it != startedGamesList->end())
        // Element found, erase it from the vector
        startedGamesList->erase(it);
}
