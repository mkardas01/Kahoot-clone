#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <poll.h>

#include "../include/data_structurs.hpp"
#include "../include/const_data.hpp"
#include "../include/check_if_user_in_game.hpp"

bool userNotInAnyGame(Games *games, User user) // Check if user is not in any game
{

    for (GameDetails &game : games->gamesList) // Iterate through games
    {

        for (User &userGame : game.users) // Iterate through users in game 
        {
            if (userGame.userID == user.userID)
                return false;
        }
    }

    return true;
}
