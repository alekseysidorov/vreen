#ifndef VK_USER_H
#define VK_USER_H

#include <QObject>

namespace vk {

class Client;
class ContactPrivate;
class Contact : public QObject
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
    Q_INVOKABLE QString photoSource(PhotoSize size = PhotoSizeMedium);
    void setPhotoSource(const QString &source, PhotoSize size = PhotoSizeMedium);
signals:
    void nameChanged(const QString &name);
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
    virtual QString name() const;
signals:
    void firstNameChanged(const QString &name);
    void lastNameChanged(const QString &name);
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

} // namespace vk

#endif // VK_USER_H
