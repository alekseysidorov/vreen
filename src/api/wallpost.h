#ifndef WALLPOST_H
#define WALLPOST_H

#include <QSharedDataPointer>
#include <QVariant>
#include "vk_global.h"
#include "attachment.h"

namespace vk {

class WallPostData;
class Client;
class Contact;

class VK_SHARED_EXPORT WallPost
{
public:  
    WallPost(Client *client = 0);
    WallPost(const WallPost &);
    WallPost &operator=(const WallPost &);
    ~WallPost();

    Client *client() const;
    void setId(int id);
    int id() const;
    void setBody(const QString &body);
    QString body() const;
    void setFromId(int id);
    int fromId() const;
    void setToId(int id);
    int toId() const;
    void setDate(const QDateTime &date);
    QDateTime date() const;
    Attachment::List attachments() const;
    Attachment::List attachments(Attachment::Type type) const;
    void setAttachments(const Attachment::List &attachmentList);

    Contact *from();
    Contact *to();

    static WallPost fromData(const QVariant data, Client *client);

    QVariant property(const QString &name, const QVariant &def = QVariant()) const;
    template<typename T>
    T property(const char *name, const T &def) const
    { return qVariantValue<T>(property(name, qVariantFromValue<T>(def))); }

    void setProperty(const QString &name, const QVariant &value);
    QStringList dynamicPropertyNames() const;
protected:
    WallPost(QVariantMap data, Client *client);
private:
    QSharedDataPointer<WallPostData> d;
};
typedef QList<WallPost> WallPostList;

} //namespace vk

#endif // WALLPOST_H
