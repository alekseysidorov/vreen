#include "buddy_p.h"

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

int Contact::id() const
{
    return d_func()->id;
}

Client *Contact::client() const
{
    return d_func()->client;
}

QString Contact::photoSource(Contact::PhotoSize size)
{
    return d_func()->sources[size];
}

void Contact::setPhotoSource(const QString &source, Contact::PhotoSize size)
{
    d_func()->sources[size] = source;
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
    return d_func()->isOnline;
}

void Buddy::setOnline(bool set)
{
    d_func()->isOnline = set;
    emit onlineChanged(set);
}

QString Buddy::name() const
{
    Q_D(const Buddy);
	return d->firstName + ' ' + d->lastName;
}

void Buddy::setTagIdList(const Buddy::TagIdList &list)
{
	d_func()->tagIdList = list;

	//TODO check tags
	emit tagsChanged(tags());
}

Buddy::TagIdList Buddy::tagIdList() const
{
	return d_func()->tagIdList;
}

QStringList Buddy::tags() const
{
	Q_D(const Buddy);
	QStringList tags;
	foreach (int id, d->tagIdList) {
		//TODO
	}
	return tags;
}

void Buddy::update(const QStringList &fields)
{
    //TODO
}

Group::Group(int id, Client *client) :
    Contact(new ContactPrivate(this, id, client))
{
}

QString Group::name() const
{
    return d_func()->name;
}

void Group::setName(const QString &name)
{
    d_func()->name = name;
    emit nameChanged(name);
}

void Group::update(const QStringList &fields)
{
    Q_UNUSED(fields);
    emit updateFinished();
}

} // namespace vk

#include "moc_buddy.cpp"
