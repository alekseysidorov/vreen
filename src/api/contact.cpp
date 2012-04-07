#include "contact_p.h"
#include "message.h"
#include "roster.h"

namespace vk {

Contact::Contact(int id, Client *client) :
    QObject(client),
    d_ptr(new ContactPrivate(this, id, client))
{
}

Contact::Contact(ContactPrivate *data) :
    QObject(data->client),
    d_ptr(data)
{
}

Contact::~Contact()
{
}

Contact::Type Contact::type()
{
    return d_func()->type;
}

void Contact::setType(Contact::Type type)
{
    Q_D(Contact);
    d->type = type;
    emit typeChanged(type);
}

int Contact::id() const
{
    return d_func()->id;
}

Client *Contact::client() const
{
    return d_func()->client;
}

QString Contact::photoSource(Contact::PhotoSize size) const
{
    Q_D(const Contact);
    return d->sources.value(size);
}

void Contact::setPhotoSource(const QString &source, Contact::PhotoSize size)
{
    d_func()->sources[size] = source;
    emit photoSourceChanged(source, size);
}

Buddy::Buddy(int id, Client *client) :
    Contact(new BuddyPrivate(this, id, client))
{
}

QString Buddy::firstName() const
{
    return d_func()->firstName;
}

void Buddy::setFirstName(const QString &firtName)
{
    Q_D(Buddy);
    d->firstName = firtName;
    emit firstNameChanged(firtName);
    emit nameChanged(name());
}

QString Buddy::lastName() const
{
    return d_func()->lastName;
}

void Buddy::setLastName(const QString &lastName)
{
    Q_D(Buddy);
    d->lastName = lastName;
    emit lastNameChanged(lastName);
    emit nameChanged(name());
}

bool Buddy::isOnline() const
{
    return d_func()->status != Offline;
}

void Buddy::setOnline(bool set)
{
    Q_D(Buddy);
    d->status = static_cast<decltype(d->status)>(set);
    emit onlineChanged(set);
}

QString Buddy::name() const
{
    Q_D(const Buddy);
    if (d->firstName.isEmpty() && d->lastName.isEmpty())
        return tr("id%1").arg(id());
    else if (d->lastName.isEmpty())
        return d->firstName;
    else
        return d->firstName + ' ' + d->lastName;

}

QStringList Buddy::tags() const
{
    Q_D(const Buddy);
    QStringList tags;
    foreach (auto data, d->tagIdList) {
        int id = data.toInt();
        tags.append(d->client->roster()->tags().value(id, tr("Unknown tag id %1").arg(id)));
    }
    return tags;
}

QString Buddy::activity() const
{
    return d_func()->activity;
}

Buddy::Status Buddy::status() const
{
    return d_func()->status;
}

void Buddy::setStatus(Buddy::Status status)
{
    d_func()->status = status;
    emit statusChanged(status);
}

/*!
 * \brief Buddy::update
 * \param fields - some fields need to update.
 * api reference \link http://vk.com/developers.php?oid=-1&p=users.get
 */
void Buddy::update(const QStringList &fields)
{
    IdList ids;
    ids.append(id());
    d_func()->client->roster()->update(ids, fields);
}

void Buddy::sendMessage(const QString &body, const QString &subject)
{
    Q_D(Buddy);
    Message message(d->client);
    message.setBody(body);
    message.setSubject(subject);
    message.setTo(this);
    d->client->sendMessage(message);
}

Group::Group(int id, Client *client) :
    Contact(new GroupPrivate(this, id, client))
{
    Q_D(Group);
    d->type = GroupType;
}

QString Group::name() const
{
    Q_D(const Group);
    if (!d->name.isEmpty())
        return d->name;
    return tr("group-%1").arg(id());
}

void Group::setName(const QString &name)
{
    d_func()->name = name;
    emit nameChanged(name);
}

} // namespace vk

#include "moc_contact.cpp"
