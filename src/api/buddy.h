#ifndef VK_USER_H
#define VK_USER_H

#include "vk_global.h"
#include <QStringList>

namespace vk {

class Client;
class ContactPrivate;
class VK_SHARED_EXPORT Contact : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Contact)
public:

    enum PhotoSize {
        PhotoSizeSmall,
        PhotoSizeMedium,
        PhotoSizeLarge
    };

    Contact(int id, Client *client);
    Contact(ContactPrivate *data);
    virtual ~Contact();

    virtual QString name() const = 0;
    int id() const;
    Client *client() const;
    Q_INVOKABLE QString photoSource(PhotoSize size = PhotoSizeSmall);
    void setPhotoSource(const QString &source, PhotoSize size = PhotoSizeSmall);
public slots:
    virtual void update(const QStringList &fields = QStringList()) = 0;
signals:
    void nameChanged(const QString &name);
    void updateFinished(bool success = true);
protected:
    QScopedPointer<ContactPrivate> d_ptr;
};

class BuddyPrivate;
class Buddy : public Contact
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Buddy)
public:
    enum NameCase {
        NomCase,
        GenCase,
        DatCase,
        AccCase,
        InsCase,
        AblCase
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
    virtual void update(const QStringList &fields = QStringList());
signals:
    void firstNameChanged(const QString &name);
    void lastNameChanged(const QString &name);
    void onlineChanged(bool isOnline);
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
    virtual void update(const QStringList &fields = QStringList());
};

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

#endif // VK_USER_H
