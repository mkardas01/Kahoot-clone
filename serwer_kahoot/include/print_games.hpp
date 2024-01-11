#ifndef PRINT_GAMESHPP
void printGames(const Games &games, UserList *userList)
{
    std::cout<<std::endl;
    std::cout<<"-------"<<std::endl;
    std::cout << "Gracze z gier" << std::endl;

    for (const auto &user : games.gamesList[1].users)
    {

        std::cout << "user: " << user.userID << " " << user.nickname << std::endl;
    }

    std::cout << "Owner: " << games.gamesList[1].gameOwnerID << std::endl;

    std::cout << "Gracze z userlist" << std::endl;

    for (User &userListc : userList->users)
    {
        std::cout << userListc.userID << " " << userListc.client_socket << " " << userListc.nickname << std::endl;
        std::cout << userList->buffer[userListc.userID] << " " << userList->eventListener[userListc.userID].fd << " " << userList->sendListener[userListc.userID].fd<<std::endl;
    }
}
#endif // PRINT_GAMESHPP
