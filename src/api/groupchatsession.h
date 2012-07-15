#ifndef VK_GROUPCHATSESSION_H
#define VK_GROUPCHATSESSION_H
#include "messagesession.h"

namespace vk {

class GroupChatSession : public vk::MessageSession
{
    Q_OBJECT
public:
    explicit GroupChatSession(int chatId, Client *parent);
};

} // namespace vk

#endif // VK_GROUPCHATSESSION_H
