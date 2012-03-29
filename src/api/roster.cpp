#include "roster_p.h"

#include <QDebug>

namespace vk {

//typedef QHash<QString, QByteArray> PropertyHash;
//PropertyHash init_property_hash()
//{
//    PropertyHash hash;
//    hash.insert("first_name", "firstName");
//    hash.insert("last_name", "lastName");
//    hash.insert("name", "name");
//    return hash;
//}
//Q_GLOBAL_STATIC_WITH_INITIALIZER(PropertyHash, propertyHash, *x = init_property_hash())

//static const char *photo_size[] = {
//    "photo",
//    "photo_medium",
//    "photo_big",
//    "photo_medium_rec",
//    "photo_big_rec"
//};

//static auto lambda = [&](const QVariant &){};
//static QHash<QString, decltype(lambda)> LambdaHash;

Roster::Roster(Client *client) :
    QObject(client),
    d_ptr(new RosterPrivate(this, client))
{

}

Roster::~Roster()
{

}

Contact *Roster::contact(int id)
{
    Q_D(Roster);
    auto contact = d->contactHash.value(id);
    if (!contact) {
        if (id > 0)
            contact = new Buddy(id, d->client);
        else
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
        if (id < 0) {
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
    auto reply = sender_cast<Reply*>(q->sender());

    auto list = response.toList();
    QStringList tags;
    foreach (auto item, list) {
        tags.append(item.toMap().value("name").toString());
    }
    q->setTags(tags);

    reply->deleteLater();
}

void RosterPrivate::_q_friends_received(const QVariant &response)
{
    Q_Q(Roster);
    auto reply = sender_cast<Reply*>(q->sender());

    foreach (auto data, response.toList())
        q->contact(data.toMap());
    reply->deleteLater();
    emit q->syncFinished(true);
}

} // namespace vk

#include "moc_roster.cpp"
