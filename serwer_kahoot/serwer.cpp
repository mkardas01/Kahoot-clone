#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctime>
#include <algorithm>
#include <poll.h>
#include <fcntl.h>
#include <thread>

#include "include/json.hpp"

using json = nlohmann::json;

#include "include/const_data.hpp"


#include "include/data_structurs.hpp"
#include "include/send_data_rank.hpp"
#include "include/print_games.hpp"
#include "include/game_create.hpp"
#include "include/question_to_json.hpp"
#include "include/send_questions_or_endofgame.hpp"
#include "include/start_game_send_player_to_lobby.hpp"
#include "include/check_if_user_in_game.hpp"
#include "include/check_nickname_availability.hpp"
#include "include/player_join_rejoin_game.hpp"
#include "include/owner_rejon.hpp"
#include "include/owner_start_lobby.hpp"
#include "include/send_players_answer_to_owner.hpp"
#include "include/menage_points.hpp"
#include "include/handle_connection.hpp"
#include "include/handle_data.hpp"
#include "include/check_if_next_question.hpp"
#include "include/message_queue.hpp"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Games games;
    UserList userList;
    std::vector<GameDetails> startedGamesList;
    MessageQueue messageQueue;

    games.generateDummyData(2); // Put sample game with id 0 and 1, gamepin is 123

    int server_socket;
    struct sockaddr_in server_addr;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // Create server socket

    if (server_socket == -1) // Handle errors
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = SERVER_ADRESS;

    // Bind the socket
    if (::bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Set socket to non blocking
    fcntl(server_socket, F_SETFL, O_NONBLOCK);

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1)
    {
        perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    
    while (1) // Handle acctions
    {
        try
        {
            checkMessageQueue(&messageQueue);

            acceptClient(&userList, server_socket);

            readData(&games, &userList, &startedGamesList, &messageQueue);

            checkIfSendNextQuestion(&games, &userList, &startedGamesList, &messageQueue);

            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Sleep for 300 ms

            // printGames(games, &userList);
        }
        catch (...)
        {
            std::cerr << "Niezidentyfikowany wyjatek" << std::endl;
        }
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
