#ifndef ROSTER_P_H
#define ROSTER_P_H
#include "roster.h"
#include "client_p.h"
#include "contact_p.h"

namespace vk {

typedef QHash<int, Contact*> ContactHash;

class Roster;
class RosterPrivate
{
    Q_DECLARE_PUBLIC(Roster)
public:
    RosterPrivate(Roster *q, Client *client) :
        q_ptr(q), client(client), owner(0)
    {}
    Roster *q_ptr;
    Client *client;
    ContactHash contactHash;
    Buddy *owner;
    QStringList tags;

    void getTags();
    void getOnline();
	void getFriends(const QVariantMap &args = QVariantMap());
    void addContact(Contact *contact);

    void _q_tags_received(const QVariant &response);
    void _q_friends_received(const QVariant &response);
    void _q_status_changed(int userId, vk::Buddy::Status status);
};

} //namespace vk

#endif // ROSTER_P_H
