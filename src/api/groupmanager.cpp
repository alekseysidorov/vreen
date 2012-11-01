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
#include "groupmanager.h"
#include "client.h"
#include "contact.h"

namespace Vreen {

class GroupManager;
class GroupManagerPrivate
{
    Q_DECLARE_PUBLIC(GroupManager)
public:
    GroupManagerPrivate(GroupManager *q, Client *client) : q_ptr(q), client(client) {}
    GroupManager *q_ptr;
    Client *client;
    QHash<int, Group*> groupHash;
};

GroupManager::GroupManager(Client *client) :
    QObject(client),
    d_ptr(new GroupManagerPrivate(this, client))
{
}

GroupManager::~GroupManager()
{
}

Client *GroupManager::client() const
{
    return d_func()->client;
}

Group *GroupManager::group(int gid)
{
    Q_D(GroupManager);
    auto group = d->groupHash.value(gid);
    if (!group) {
        group = new Group(gid, client());
        d->groupHash.insert(gid, group);
    }
    return group;
}

Group *GroupManager::group(int gid) const
{
    return d_func()->groupHash.value(gid);
}

} // namespace Vreen

