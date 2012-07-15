#include "groupchatsession.h"
#include "messagesession_p.h"

namespace vk {

class GroupChatSession;
class GroupChatSessionPrivate : public MessageSessionPrivate
{
    Q_DECLARE_PUBLIC(GroupChatSession)
public:
    GroupChatSessionPrivate(MessageSession *q, Client *client, int uid) :
        MessageSessionPrivate(q, client, uid) {}
};


GroupChatSession::GroupChatSession(int chatId, Client *client) :
    MessageSession(new GroupChatSessionPrivate(this, client, chatId))
{
}

} // namespace vk
