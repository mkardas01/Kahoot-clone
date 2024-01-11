#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

void checkMessageQueue(MessageQueue* messageQueue); // Check if it's time to send new question to players
void deleteMessage(MessageQueue* messageQueue, int i);
#endif // MESSAGE_QUEUE_HPP
