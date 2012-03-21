#ifndef CLIENT_P_H
#define CLIENT_P_H

#include "client.h"

namespace vk {

class ClientPrivate
{
    Q_DECLARE_PUBLIC(Client)
public:
    ClientPrivate(Client *q) : q_ptr(q) {}
    Client *q_ptr;
    QString login;
    QString password;
};

} //namespace vk

#endif // CLIENT_P_H
