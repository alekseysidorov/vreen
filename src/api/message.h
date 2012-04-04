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
		Read,
		Unread,
		Unknown = -1
    };
    enum Direction {
		In,
		Out,
		Forward = -1
    };

    Message(Client *client = 0);
    Message(const QVariantMap &data, Client *client);
    Message(const Message &other);
    Message &operator =(const Message &other);
    bool operator ==(const Message &other);
    virtual ~Message();

    Client *client() const;

	int id() const;
	void setId(int id);
    QDateTime date() const;
    void setDate(const QDateTime &date);
    Contact *from() const;
    void setFrom(Contact *from);
    Contact *to() const;
    void setTo(Contact *to);
    QString subject() const;
    void setSubject(const QString &subject);
    QString body() const;
    void setBody(const QString &body);
    ReadState readState() const;
    void setReadState(ReadState state);
    Direction direction() const;
    void setDirection(Direction direction);
protected:
    QSharedDataPointer<MessageData> d;
};

typedef QList<int> IdList;

} // namespace vk

Q_DECLARE_METATYPE(vk::Message)
Q_DECLARE_METATYPE(vk::MessageList)
Q_DECLARE_METATYPE(vk::IdList)

#endif // VK_MESSAGE_H
