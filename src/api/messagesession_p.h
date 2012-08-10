#ifndef MESSAGESESSION_P_H
#define MESSAGESESSION_P_H
#include "messagesession.h"
#include "longpoll.h"

namespace Vreen {

class MessageSession;
class MessageSessionPrivate
{
    Q_DECLARE_PUBLIC(MessageSession)
public:
    MessageSessionPrivate(MessageSession *q, Client *client, int uid) :
        q_ptr(q), client(client), uid(uid) {}
    MessageSession *q_ptr;
    Client *client;
    int uid;
};

} //namespace Vreen

#endif // MESSAGESESSION_P_H
