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

	Q_PROPERTY(QString name READ name NOTIFY nameChanged)
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
	Q_INVOKABLE int id() const;
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

	Q_PROPERTY(QString fistName READ firstName NOTIFY firstNameChanged)
	Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
	Q_PROPERTY(bool isOnline READ isOnline NOTIFY onlineChanged)
	Q_PROPERTY(QStringList tags READ tags NOTIFY tagsChanged)

public:
    enum NameCase {
        NomCase,
        GenCase,
        DatCase,
        AccCase,
        InsCase,
        AblCase
    };
	typedef QList<int> TagIdList;

    Buddy(int id, Client *client);
    //TODO name case support maybe needed
    QString firstName() const;
    void setFirstName(const QString &firstName);
    QString lastName() const;
    void setLastName(const QString &lastName);
    bool isOnline() const;
    void setOnline(bool set);
    virtual QString name() const;
	void setTagIdList(const TagIdList &list);
	TagIdList tagIdList() const;
	QStringList tags() const;

    virtual void update(const QStringList &fields = QStringList());
signals:
    void firstNameChanged(const QString &name);
    void lastNameChanged(const QString &name);
    void onlineChanged(bool isOnline);
	void tagsChanged(const QStringList &tags);
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
