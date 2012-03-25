#ifndef VK_MESSAGE_H
#define VK_MESSAGE_H
#include <QVariant>
#include <QSharedData>

namespace vk {

class Client;
class MessageData;
class Message
{
public:
    Message(Client *client);
protected:
    QSharedDataPointer<MessageData> d;
};

} // namespace vk

#endif // VK_MESSAGE_H
