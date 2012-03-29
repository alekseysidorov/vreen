#ifndef USER_P_H
#define USER_P_H
#include "buddy.h"
#include "client.h"
#include <QStringList>
#include <QVector>

namespace vk {

class Contact;
class ContactPrivate
{
    Q_DECLARE_PUBLIC(Contact)
public:
    ContactPrivate(Contact *q, int id, Client *client) : q_ptr(q),
        client(client), id(id), sources(Contact::PhotoSizeLargeRec) {}
    Contact *q_ptr;
    Client *client;
    int id;
    QVector<QString> sources;

    QString smallSource() const { return sources[Contact::PhotoSizeSmall]; }
    void setSmallSource(const QString &source)
    {
        q_func()->setPhotoSource(source, Contact::PhotoSizeSmall);
    }
};

class BuddyPrivate : public ContactPrivate
{
    Q_DECLARE_PUBLIC(Buddy)
public:
    BuddyPrivate(Contact *q, int id, Client *client) : ContactPrivate(q, id, client), isOnline(false) {}
    QString firstName;
    QString lastName;
    bool isOnline;
	Buddy::TagIdList tagIdList;
};

class GroupPrivate : public ContactPrivate
{
    Q_DECLARE_PUBLIC(Group)
public:
    GroupPrivate(Contact *q, int id, Client *client) : ContactPrivate(q, id, client) {}
    QString name;
};

} //namespace vk

#endif // USER_P_H
