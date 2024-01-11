#include <poll.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include "../include/const_data.hpp"
#include "../include/data_structurs.hpp"

#include "../include/message_queue.hpp"

void checkMessageQueue(MessageQueue *messageQueue) // send message to specific user
{
    int i = 0;
    for (const User &user : messageQueue->users)
    {
        auto currentTime = std::chrono::steady_clock::now();

        if ((currentTime - messageQueue->startTime[i]) <= std::chrono::seconds(3))
        {

            int poll_users = poll(&messageQueue->sendListener[i], 1, 0);

            if (poll_users > 0 && (messageQueue->sendListener[user.userID].revents & POLLOUT)) // Check if message can be send
            {
                size_t dataSize = strlen(messageQueue->messages[i].c_str());
                ssize_t sentBytes = send(messageQueue->sendListener[user.userID].fd, messageQueue->messages[i].c_str(), dataSize, 0); // Send message

                if (sentBytes == -1) // Check for errors
                {
                    perror("Error sending data");
                }
                
                deleteMessage(messageQueue, i);
            }
        }
        else
        {
            deleteMessage(messageQueue, i);
        }
    }
}

void deleteMessage(MessageQueue *messageQueue, int i){
    messageQueue->messages.erase(messageQueue->messages.begin() + i);
    messageQueue->startTime.erase(messageQueue->startTime.begin() + i);
    messageQueue->users.erase(messageQueue->users.begin() + i);
    messageQueue->sendListener.erase(messageQueue->sendListener.begin() + i);

}