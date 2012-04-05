#ifndef CHATSESSION_P_H
#define CHATSESSION_P_H
#include "chatsession.h"

namespace vk {

class ChatSessionPrivate
{
    Q_DECLARE_PUBLIC(ChatSession)
public:
    ChatSessionPrivate(ChatSession *q, Contact *contact) : q_ptr(q),
        contact(contact), isActive(false) {}
    ChatSession *q_ptr;

    Contact *contact;
    bool isActive;

    void _q_history_received(const QVariant &);
    void _q_message_read_state_updated(const QVariant &);
    void _q_message_added(const Message &message);
};

} //namespace vk


#endif // CHATSESSION_P_H
