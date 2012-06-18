#ifndef VK_MESSAGE_H
#define VK_MESSAGE_H
#include <QVariant>
#include <QSharedData>
#include "attachment.h"

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
    Q_ENUMS(Flags)
public:
    enum Flag {
        FlagUnread   = 1,
        FlagOutbox   = 2,
        FlagReplied  = 4,
        FlagImportant= 8,
        FlagChat     = 16,
        FlagFriends  = 32,
        FlagSpam     = 64,
        FlagDeleted  = 128,
        FlagFixed    = 256,
        FlagMedia    = 512
    };
    Q_DECLARE_FLAGS(Flags, Flag)

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
	int fromId() const;
	int toId() const;
    Contact *from() const;
    void setFrom(Contact *from);
    Contact *to() const;
    void setTo(Contact *to);
    QString subject() const;
    void setSubject(const QString &subject);
    QString body() const;
    void setBody(const QString &body);
    bool isUnread() const;
    void setUnread(bool set);
    bool isIncoming() const;
    void setIncoming(bool set);
    void setFlags(Flags flags);
    Flags flags() const;
    void setFlag(Flag flag, bool set = true);
    bool testFlag(Flag flag) const;
    Attachment::Hash attachments() const;
    Attachment::List attachments(Attachment::Type type) const;
    void setAttachments(const Attachment::List &attachmentList);
protected:
    QSharedDataPointer<MessageData> d;
};

typedef QList<int> IdList;

} // namespace vk

Q_DECLARE_METATYPE(vk::Message)
Q_DECLARE_METATYPE(vk::MessageList)
Q_DECLARE_METATYPE(vk::IdList)
Q_DECLARE_OPERATORS_FOR_FLAGS(vk::Message::Flags)

#endif // VK_MESSAGE_H
