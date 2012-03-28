#include "roster_p.h"

namespace vk {

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

QStringList Roster::tags() const
{
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

	reply->deleteLater();
}

} // namespace vk

#include "moc_roster.cpp"
