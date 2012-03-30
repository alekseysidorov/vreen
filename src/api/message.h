#ifndef VK_MESSAGE_H
#define VK_MESSAGE_H
#include <QVariant>
#include <QSharedData>
#include "vk_global.h"

namespace vk {

class Contact;
class Message;
typedef QList<Message> MessageList;

class Client;
class MessageData;
class VK_SHARED_EXPORT Message
{
    Q_GADGET
    Q_ENUMS(ReadState)
    Q_ENUMS(Direction)
public:
    enum ReadState {
        MessageRead,
        MessageUnread,
        MessageUnknown = -1
    };
    enum Direction {
        MessageIn,
        MessageOut,
        MessageFwd = -1
    };

    Message(Client *client = 0);
    Message(const QVariantMap &data, Client *client);
    Message(const Message &other);
    Message &operator =(const Message &other);
    bool operator ==(const Message &other);
    virtual ~Message();

    int id() const;
    Client *client() const;

    QDateTime date() const;
    void setDate(const QDateTime &date);
    Contact *contact() const;
    void setContact(Contact *contact);
    QString title() const;
    void setTitle(const QString &title);
    QString body() const;
    void setBody(const QString &body);
    ReadState readState() const;
    void setReadState(ReadState state);
    Direction direction() const;
    void setDirection(Direction direction);
protected:
    QSharedDataPointer<MessageData> d;
};

} // namespace vk

Q_DECLARE_METATYPE(vk::Message)
Q_DECLARE_METATYPE(vk::MessageList)

#endif // VK_MESSAGE_H
