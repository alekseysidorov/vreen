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
    connect(d->client->longPoll(), SIGNAL(contactStatusChanged(int, vk::Buddy::Status)),
            this, SLOT(_q_status_changed(int, vk::Buddy::Status)));
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
    qDeleteAll(d->contactHash);
    d->owner = new Buddy(uid, d->client);
    d->owner->setType(Contact::UserType);
    emit uidChanged(uid);
    d->addContact(d->owner);
}

int Roster::uid() const
{
    return d_func()->owner->id();
}

Contact *Roster::owner() const
{
    return d_func()->owner;
}

Contact *Roster::contact(int id, Contact::Type type)
{
    Q_D(Roster);
    if (!id) {
        qWarning("Contact id cannot be null!");
        return 0;
    }
    auto contact = d->contactHash.value(id);
    if (!contact) {
        if (d->owner && d->owner->id() == id)
            return d->owner;
        if (type == Contact::GroupType) {
            auto group = new Group(id, d->client);
            contact = group;
        } else {
            auto buddy = new Buddy(id, d->client);
            buddy->setType(type);
            contact = buddy;
        }
        d->addContact(contact);
    }
    return contact;
}

Contact *Roster::contact(int id) const
{
    return d_func()->contactHash.value(id);
}

ContactList Roster::contacts() const
{
    return d_func()->contactHash.values();
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

void RosterPrivate::addContact(Contact *contact)
{
    Q_Q(Roster);
    emit q->contactAdded(contact);
    switch (contact->type()) {
    case Contact::FriendType:
        emit q->friendAdded(contact);
        break;
    case Contact::BuddyType:
    case Contact::UserType: {
        IdList ids;
        ids.append(contact->id());
        q->update(ids, QStringList() << VK_COMMON_FIELDS); //TODO move!
        break;
    }
    default:
        break;
    }
    contactHash.insert(contact->id(), contact);
}

void Roster::fillContact(Contact *contact, const QVariantMap &data)
{
    auto it = data.constBegin();
    for (; it != data.constEnd(); it++) {
        QByteArray property = "_q_" + it.key().toLatin1();
        contact->setProperty(property.data(), it.value());
    }
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
        auto contact = contactHash.value(id);
        if (!contact) {
            auto contact = new Buddy(id, client);
            if (isFriend)
                contact->setType(Contact::FriendType);
            q->fillContact(contact, map);
            addContact(contact);
        } else {
            q->fillContact(contact, map);
            if (isFriend && contact->type() != Contact::FriendType) {
                contact->setType(Contact::FriendType);
                emit q->friendAdded(contact);
            }
        }
    }
    emit q->syncFinished(true);
}

void RosterPrivate::_q_status_changed(int userId, Buddy::Status status)
{
    Q_Q(Roster);
    auto buddy = contact_cast<Buddy*>(q->contact(userId));
    if (buddy)
        buddy->setStatus(status);
}

} // namespace vk

#include "moc_roster.cpp"
