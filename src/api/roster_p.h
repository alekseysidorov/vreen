#ifndef ROSTER_P_H
#define ROSTER_P_H
#include "roster.h"
#include "client_p.h"

namespace vk {

class Roster;
class RosterPrivate
{
    Q_DECLARE_PUBLIC(Roster)
public:
    RosterPrivate(Roster *q, Client *client) : q_ptr(q), client(client) {}
    Roster *q_ptr;
    Client *client;
};

} //namespace vk

#endif // ROSTER_P_H
