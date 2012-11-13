/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
#ifndef ROSTER_P_H
#define ROSTER_P_H
#include "roster.h"
#include "client_p.h"
#include "contact_p.h"

namespace Vreen {

typedef QHash<int, Buddy*> BuddyHash;

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
    BuddyHash buddyHash;
    Buddy *owner;
    QStringList tags;

    void getTags();
    void getOnline();
    void getFriends(const QVariantMap &args = QVariantMap());
    void addBuddy(Buddy *contact);

	static QVariant handleGetRequests(const QVariant &response);

    void _q_tags_received(const QVariant &response);
    void _q_friends_received(const QVariant &response);
    void _q_status_changed(int userId, Vreen::Contact::Status status);
    void _q_online_changed(bool);
};

} //namespace Vreen

#endif // ROSTER_P_H

