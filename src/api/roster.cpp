/****************************************************************************
**
** VKit - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VKIT_BEGIN_LICENSE$
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
** $VKIT_END_LICENSE$
**
****************************************************************************/
#include "roster_p.h"
#include "longpoll.h"

#include <QDebug>
#include <QTimer>

namespace vk {

/*!
 * \brief The Roster class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=friends.get
 */

/*!
 * \brief Roster::Roster
 * \param client
 */
Roster::Roster(Client *client, int uid) :
    QObject(client),
    d_ptr(new RosterPrivate(this, client))
{
    Q_D(Roster);
    connect(d->client->longPoll(), SIGNAL(contactStatusChanged(int, vk::Contact::Status)),
            this, SLOT(_q_status_changed(int, vk::Contact::Status)));
    connect(d->client, SIGNAL(onlineStateChanged(bool)), SLOT(_q_online_changed(bool)));
    if (uid)
        setUid(uid);
}

Roster::~Roster()
{

}

void Roster::setUid(int uid)
{
    Q_D(Roster);
    if (d->owner && uid == d->owner->id())
        return;
    qDeleteAll(d->buddyHash);
    d->owner = new Buddy(uid, d->client);
    emit uidChanged(uid);
    d->addBuddy(d->owner);
}

int Roster::uid() const
{
    Q_D(const Roster);
    if (d->owner)
        return d->owner->id();
    return 0;
}

Buddy *Roster::owner() const
{
    return d_func()->owner;
}

Buddy *Roster::buddy(int id)
{
    Q_D(Roster);
    if (!id) {
        qWarning("Contact id cannot be null!");
        return 0;
    }
    if (id < 0) {
        id = qAbs(id);
        qWarning("For groups use class GroupManager");
    }


    auto buddy = d->buddyHash.value(id);
    if (!buddy) {
        if (d->owner && d->owner->id() == id)
            return d->owner;
            buddy = new Buddy(id, d->client);
        d->addBuddy(buddy);
    }
    return buddy;
}

Buddy *Roster::buddy(int id) const
{
    return d_func()->buddyHash.value(id);
}

BuddyList Roster::buddies() const
{
    return d_func()->buddyHash.values();
}

QStringList Roster::tags() const
{
    return d_func()->tags;
}

void Roster::setTags(const QStringList &tags)
{
    d_func()->tags = tags;
    emit tagsChanged(tags);
}

void Roster::sync(const QStringList &fields)
{
    Q_D(Roster);
    //TODO rewrite with method chains with lambdas in Qt5
    QVariantMap args;
    args.insert("fields", fields.join(","));
    args.insert("order", "hints");

    d->getTags();
    d->getFriends(args);
}

/*!
 * \brief Roster::update
 * \param ids
 * \param fields from \link http://vk.com/developers.php?oid=-1&p=Описание_полей_параметра_fields
 */
void Roster::update(const IdList &ids, const QStringList &fields)
{
    Q_D(Roster);
    QVariantMap args;
    args.insert("uids", join(ids));
    args.insert("fields", fields.join(","));
    auto reply = d->client->request("users.get", args);
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   this, SLOT(_q_friends_received(const QVariant&)));
}

void RosterPrivate::getTags()
{
    Q_Q(Roster);
    auto reply = client->request("friends.getLists");
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   q, SLOT(_q_tags_received(const QVariant&)));
}

void RosterPrivate::getOnline()
{
}

void RosterPrivate::getFriends(const QVariantMap &args)
{
    Q_Q(Roster);
    auto reply = client->request("friends.get", args);
    reply->setProperty("friend", true);
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   q, SLOT(_q_friends_received(const QVariant&)));
}

void RosterPrivate::addBuddy(Buddy *buddy)
{
    Q_Q(Roster);
    emit q->buddyAdded(buddy);
    if (buddy->isFriend())
        emit q->friendAdded(static_cast<Buddy*>(buddy));
    else {
        IdList ids;
        ids.append(buddy->id());
        q->update(ids, QStringList() << VK_COMMON_FIELDS); //TODO move!
    }
    buddyHash.insert(buddy->id(), buddy);
}

void RosterPrivate::_q_tags_received(const QVariant &response)
{
    Q_Q(Roster);
    auto list = response.toList();
    QStringList tags;
    foreach (auto item, list) {
        tags.append(item.toMap().value("name").toString());
    }
    q->setTags(tags);
}

void RosterPrivate::_q_friends_received(const QVariant &response)
{
    Q_Q(Roster);
    bool isFriend = q->sender()->property("friend").toBool();
    foreach (auto data, response.toList()) {
        auto map = data.toMap();
        int id = map.value("uid").toInt();
        auto buddy = buddyHash.value(id);
        if (!buddy) {
            auto buddy = new Buddy(id, client);
            buddy->setIsFriend(isFriend);
            Contact::fillContact(buddy, map);
            addBuddy(buddy);
        } else {
            Contact::fillContact(buddy, map);
            if (isFriend) {
                buddy->setIsFriend(isFriend);
                emit q->friendAdded(buddy);
            }
        }
    }
    emit q->syncFinished(true);
}

void RosterPrivate::_q_status_changed(int userId, Buddy::Status status)
{
    Q_Q(Roster);
    auto buddy = contact_cast<Buddy*>(q->buddy(userId));
    if (buddy)
        buddy->setStatus(status);
}

void RosterPrivate::_q_online_changed(bool set)
{
    if (!set)
        foreach(auto contact, buddyHash)
            if (auto buddy = contact_cast<Buddy*>(contact))
                buddy->setOnline(false);

}

} // namespace vk

#include "moc_roster.cpp"

