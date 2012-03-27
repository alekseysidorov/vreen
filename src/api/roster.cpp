#include "roster_p.h"

namespace vk {

Roster::Roster(Client *client) :
    QObject(client),
    d_ptr(new RosterPrivate(this, client))
{

}

Roster::~Roster()
{

}

Contact *Roster::contact(int id) const
{
}

Contact *Roster::contact(const QVariantMap &data)
{
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

void Roster::sync(const QStringList &filter)
{
    Q_D(Roster);
}

void RosterPrivate::getTags()
{
}

void RosterPrivate::getOnline()
{
}

void RosterPrivate::_q_tags_received(const QVariant &response)
{
}

void RosterPrivate::_q_friends_received(const QVariant &response)
{
}

} // namespace vk

#include "moc_roster.cpp"
