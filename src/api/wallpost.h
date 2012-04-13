#ifndef WALLPOST_H
#define WALLPOST_H

#include <QSharedDataPointer>
#include <QVariant>
#include "vk_global.h"

namespace vk {

class WallPostData;
class Client;
class Contact;

class VK_SHARED_EXPORT WallPost
{
public:
    WallPost(Client *client = 0);
    WallPost(const QVariantMap d, Client *client);
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

    Contact *from();
    Contact *to();
private:
    QSharedDataPointer<WallPostData> d;
};
typedef QList<WallPost> WallPostList;

} //namespace vk

#endif // WALLPOST_H
