#include "roster_p.h"
#include "longpoll.h"

#include <QDebug>

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
    d->owner->update(QStringList() << VK_ALL_FIELDS); //TODO move!
    emit uidChanged(uid);
}

int Roster::uid() const
{
    return d_func()->owner->id();
}

Contact *Roster::owner() const
{
    return d_func()->owner;
}

Contact *Roster::contact(int id)
{
    Q_D(Roster);
    auto contact = d->contactHash.value(id);
    if (!contact) {
        if (d->owner && d->owner->id() == id)
            return d->owner;
        if (id > 0) {
            auto buddy = new Buddy(id, d->client);
            buddy->update(QStringList() << VK_COMMON_FIELDS); //TODO move!
            contact = buddy;
        } else
            contact = new Group(id, d->client);
    }
    return contact;
}

Contact *Roster::contact(int id) const
{
    return d_func()->contactHash.value(id);
}

Contact *Roster::contact(const QVariantMap &data)
{
    Q_D(Roster);
    int id = data.value("uid").toInt();
    if (!id) {
        qWarning("Contact id cannot be null!");
        return 0;
    }
    auto contact = d->contactHash.value(id);
    if (!contact) {
        if (d->owner && d->owner->id() == id) {
            d->fillContact(d->owner, data);
            return d->owner;
        } else if (id < 0) {
            contact = new Group(id, d->client);
        } else {
            contact = new Buddy(id, d->client);
        }
        d->contactHash.insert(id, contact);
        d->fillContact(contact, data);
        emit contactAdded(contact);
    }
    d->fillContact(contact, data);
    return contact;
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
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   q, SLOT(_q_friends_received(const QVariant&)));
}

void RosterPrivate::fillContact(Contact *contact, const QVariantMap &data)
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
    foreach (auto data, response.toList())
        q->contact(data.toMap());
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
