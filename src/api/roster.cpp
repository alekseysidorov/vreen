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

void Roster::sync(const QStringList &filter)
{
    Q_D(Roster);
}

} // namespace vk

#include "moc_roster.cpp"
