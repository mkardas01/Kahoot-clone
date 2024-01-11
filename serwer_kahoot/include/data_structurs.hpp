

struct User
{
    std::string nickname;
    int userID;
    int client_socket;
    int points;

    User()
        : nickname(""), points(0){

                     };
};

struct MessageQueue{
    std::vector<std::string> messages;
    std::vector<User> users;
    std::vector<pollfd> sendListener;
    std::vector<std::chrono::steady_clock::time_point> startTime;

};

struct UserList
{
    std::vector<pollfd> eventListener;
    std::vector<pollfd> sendListener;
    std::vector<User> users;
    std::vector<std::string> buffer;
};

struct Question
{
    std::string question;
    std::string answers[4];
    int indexOfCorrectAnswer;
};

struct GameDetails
{
    int gameID;
    int gamePin;
    int currentQuestion;
    int questionsNumber;
    int gameOwnerID;
    int usersWhoAnswered;
    std::chrono::steady_clock::time_point startTime;
    std::string gameStatus;          // notWaitingForPlayers, waitingForPlayers, started
    std::vector<Question> questions; // game questions
    std::vector<User> users;         // game users

    GameDetails()
        : currentQuestion(0), questionsNumber(0), gameOwnerID(-2), usersWhoAnswered(0), gameStatus("notWaitingForPlayers"){

                                                                                      };
};

struct Games
{
    std::vector<GameDetails> gamesList; // zawiera gamedetails

    void generateDummyData(int numberOfGames)
    {
        for (int i = 0; i < numberOfGames; ++i)
        {
            GameDetails game;
            game.gameID = i;
            game.gamePin = 123;       // Losowy PIN do gry
            game.questionsNumber = 4; // Losowa liczba pytań

            // Generowanie pytań dla gry
            for (int j = 0; j < game.questionsNumber; ++j)
            {
                Question q;
                q.question = "Question " + std::to_string(j + 1);
                for (int k = 0; k < 4; ++k)
                {
                    q.answers[k] = "Answer " + std::to_string(j + 1);
                }
                q.indexOfCorrectAnswer = 0;
                game.questions.push_back(q);
            }

            gamesList.push_back(game);
        }
    }
};