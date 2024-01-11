#include <string>
#include <poll.h>
#include <chrono>
#include <vector>
#include "../include/data_structurs.hpp"
#include "../include/const_data.hpp"
#include "../include/check_nickname_availability.hpp"

std::string nickNameStatus(Games *games, int gameID, std::string &gameNickname) // Check nickname status
{
    // Check if the gameID is valid
    if (gameID < 0 || gameID >= static_cast<int>(games->gamesList.size()))
    {
        // Handle invalid gameID
        return ERROR; // error
    }

    // Iterate over the users in the specified game
    for (const User &user : games->gamesList[gameID].users)
    {
        if (user.nickname == gameNickname)
        {

            if (user.userID != -1)
                return UNAVAILABLE; // Cant rejoin user is connected to this nickname
            else
                return REJOIN; // Can rejoin, user is not connectd to this nickname
        }
    }

    // The nickname is available
    return AVAILABLE;
}
