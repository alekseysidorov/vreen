#ifndef ROSTER_P_H
#define ROSTER_P_H
#include "roster.h"
#include "client_p.h"
#include "buddy_p.h"

namespace vk {

typedef QHash<int, Contact*> ContactHash;

class Roster;
class RosterPrivate
{
    Q_DECLARE_PUBLIC(Roster)
public:
    RosterPrivate(Roster *q, Client *client) :
        q_ptr(q), client(client),
        me(new Buddy(0, client))
    {}
    Roster *q_ptr;
    Client *client;
    ContactHash contactHash;
    Contact *me;
    QStringList tags;

    void getTags();
    void getOnline();
	void getFriends(const QVariantMap &args = QVariantMap());
    void fillContact(Contact *contact, const QVariantMap &data);

    void _q_tags_received(const QVariant &response);
    void _q_friends_received(const QVariant &response);
};

} //namespace vk

#endif // ROSTER_P_H
