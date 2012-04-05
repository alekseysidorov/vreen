#ifndef VK_USER_H
#define VK_USER_H

#include "vk_global.h"
#include <QStringList>
#include <QVariant>

namespace vk {

#define VK_COMMON_FIELDS QLatin1String("first_name") \
    << QLatin1String("last_name") \
    << QLatin1String("online") \
    << QLatin1String("photo") \
    << QLatin1String("photo_big") \
    << QLatin1String("photo_big_rec") \
    << QLatin1String("lists") \
    << QLatin1String("activity")

#define VK_EXTENDED_FIELDS QLatin1String("sex") \
    << QLatin1String("bdate") \
    << QLatin1String("city") \
    << QLatin1String("country") \
    << QLatin1String("education") \
    << QLatin1String("can_post") \
    << QLatin1String("contacts") \
    << QLatin1String("can_see_all_posts") \
    << QLatin1String("can_write_private_message") \
    << QLatin1String("last_seen") \
    << QLatin1String("relation") \
    << QLatin1String("nickname") \
    << QLatin1String("wall_comments") \

#define VK_ALL_FIELDS VK_COMMON_FIELDS \
    << VK_EXTENDED_FIELDS

class Client;
class ContactPrivate;
class VK_SHARED_EXPORT Contact : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Contact)

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PRIVATE_PROPERTY(Contact::d_func(), QString photoSource READ defaultSource NOTIFY photoSourceChanged)

    Q_PRIVATE_PROPERTY(Contact::d_func(), QString _q_photo READ smallSource WRITE setSmallSource DESIGNABLE false)
    Q_PRIVATE_PROPERTY(Contact::d_func(), QString _q_photo_big READ bigSource WRITE setBigSource DESIGNABLE false)
    Q_PRIVATE_PROPERTY(Contact::d_func(), QString _q_photo_big_rec READ bigSourceRec WRITE setBigSourceRec DESIGNABLE false)
public:

    enum PhotoSize {
        PhotoSizeSmall,
        PhotoSizeMedium,
        PhotoSizeBig,
        PhotoSizeMediumRec,
        PhotoSizeBigRec
    };

    Contact(int id, Client *client);
    Contact(ContactPrivate *data);
    virtual ~Contact();

    virtual QString name() const = 0;
    int id() const;
    Client *client() const;
    Q_INVOKABLE QString photoSource(PhotoSize size = PhotoSizeSmall) const;
    void setPhotoSource(const QString &source, PhotoSize size = PhotoSizeSmall);
signals:
    void nameChanged(const QString &name);
    void photoSourceChanged(const QString &source, PhotoSize);
protected:
    QScopedPointer<ContactPrivate> d_ptr;
};

class BuddyPrivate;
class Buddy : public Contact
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Buddy)

    Q_PROPERTY(QString fistName READ firstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
    Q_PROPERTY(bool online READ isOnline NOTIFY onlineChanged)
    Q_PROPERTY(QStringList tags READ tags NOTIFY tagsChanged)
    Q_PROPERTY(QString activity READ activity NOTIFY activityChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

    //private properties
    Q_PROPERTY(QString _q_first_name READ firstName WRITE setFirstName DESIGNABLE false)
    Q_PROPERTY(QString _q_last_name READ lastName WRITE setLastName DESIGNABLE false)
    Q_PROPERTY(bool _q_online READ isOnline WRITE setOnline DESIGNABLE false)
    Q_PRIVATE_PROPERTY(d_func(), QVariantList _q_lists READ lists WRITE setLists DESIGNABLE false)
    Q_PRIVATE_PROPERTY(d_func(), QString _q_activity READ getActivity WRITE setActivity DESIGNABLE false)
public:
    enum NameCase {
        NomCase,
        GenCase,
        DatCase,
        AccCase,
        InsCase,
        AblCase
    };

    enum Status {
        Offline,
        Online,
        Away
    };

    Buddy(int id, Client *client);
    //TODO name case support maybe needed
    QString firstName() const;
    void setFirstName(const QString &firstName);
    QString lastName() const;
    void setLastName(const QString &lastName);
    bool isOnline() const;
    void setOnline(bool set);
    virtual QString name() const;
    QStringList tags() const;
    QString activity() const;
    Status status() const;
    void setStatus(Status status);

public slots:
    void update(const QStringList &fields = QStringList()
            << VK_ALL_FIELDS);
    void sendMessage(const QString &body, const QString &subject = QString());
signals:
    void firstNameChanged(const QString &name);
    void lastNameChanged(const QString &name);
    void onlineChanged(bool isOnline);
    void tagsChanged(const QStringList &tags);
    void activityChanged(const QString &activity);
    void statusChanged(Status);
};

class GroupPrivate;
class Group : public Contact
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Group)
public:
    Group(int id, Client *client);
    virtual QString name() const;
    void setName(const QString &name);
};

//TODO group chats
class GroupChat;

typedef QList<Contact*> ContactList;
typedef QList<Buddy*> BuddyList;
typedef QList<Group*> GroupList;

//contact's casts
template <typename T>
Q_INLINE_TEMPLATE T contact_cast(Contact *contact)
{
    return qobject_cast<T>(contact);
}

//fast specialization
template <>
Q_INLINE_TEMPLATE Buddy* contact_cast(Contact *contact)
{
    return contact->id() > 0 ? static_cast<Buddy*>(contact)
                             : 0;
}
template <>
Q_INLINE_TEMPLATE Group* contact_cast(Contact *contact)
{
    return contact->id() < 0 ? static_cast<Group*>(contact)
                             : 0;
}

} // namespace vk

Q_DECLARE_METATYPE(vk::Contact*)

#endif // VK_USER_H
