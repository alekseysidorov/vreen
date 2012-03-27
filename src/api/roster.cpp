#include "roster_p.h"

namespace vk {

static auto lambda = [&](const QVariant &){};

static QHash<QString, decltype(lambda)> LambdaHash;

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
    if (!contact)
        if (id > 0)
            contact = new Buddy(id, d->client);
        else
            contact = new Group(id, d->client);
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
    if (!id)
        return 0;

    if (id < 0) {
        auto group = new Group(id, d->client);
        //TODO fill
        return group;
    }
    auto buddy = new Buddy(id, d->client);
    //TODO fill
    buddy->setFirstName(data.value("first_name").toString());
    buddy->setLastName(data.value("last_name").toString());

    return buddy;
}

void Roster::sync(const QStringList &fields)
{
    Q_D(Roster);
    d->getTags();
}

} // namespace vk

#include "moc_roster.cpp"
