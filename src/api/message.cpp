#include "message.h"
#include "buddy.h"
#include "client.h"
#include "roster.h"
#include <QDateTime>

namespace vk {

class MessageData : public QSharedData
{
public:
    MessageData(Client *client) :
        client(client),
        id(0),
        contact(client->me()),
        readState(Message::MessageUnknown),
        direction(Message::MessageFwd),
        chatId(0),
        userCount(0),
        latitude(-1),
        longitude(-1)
    {}
    MessageData(const MessageData &o) :
        QSharedData(o),
        client(o.client),
        id(o.id),
        contact(o.contact),
        date(o.date),
        readState(o.readState),
        title(o.title),
        body(o.body),
        forwardMsgIds(o.forwardMsgIds),
        chatId(o.chatId),
        chatActive(o.chatActive),
        userCount(o.userCount),
        admin(o.admin),
        latitude(o.latitude),
        longitude(o.longitude)
    {}
    ~MessageData() {}

    Client *client;
    int id;
    QWeakPointer<Contact> contact;
    QDateTime date;
    Message::ReadState readState;
    Message::Direction direction;
    QString title;
    QString body;
    QList<int> forwardMsgIds;
    int chatId;
    QList<int> chatActive;
    int userCount;
    QWeakPointer<Contact> admin;
    qreal latitude;
    qreal longitude;

    template<typename T>
    T flag_helper(const QVariant &data)
    {
        int flag = -1;
        if (!data.isNull())
            flag = data.toInt();
        return static_cast<T>(flag);

    }
    void fill(const QVariantMap &data)
    {
        id = data.value("mid").toInt();
        contact = client->roster()->contact(data.value("uid").toInt());
        date = QDateTime::fromTime_t(data.value("date").toInt());
        readState = flag_helper<Message::ReadState>(data.value("read_state"));
        direction = flag_helper<Message::Direction>(data.value("out"));
        title = data.value("title").toString();
        body = data.value("body").toString();
        //TODO forward messages
        //TODO attachments
        //TODO groupchats
    }
};

Message::Message(Client *client) :
    d(new MessageData(client))
{
}

Message::Message(const QVariantMap &data, Client *client) :
    d(new MessageData(client))
{
    d->fill(data);
}

Message::Message(const Message &other) : d(other.d)
{
}

Message &Message::operator =(const Message &other)
{
    d = other.d;
    return *this;
}

bool Message::operator ==(const Message &other)
{
    return id() == other.id();
}

Message::~Message()
{
}

int Message::id() const
{
    return d->id;
}

Client *Message::client() const
{
    return d->client;
}

QDateTime Message::date() const
{
    return d->date;
}

void Message::setDate(const QDateTime &date)
{
    d->date = date;
}

Contact *Message::contact() const
{
    return d->contact.data();
}

void Message::setContact(Contact *contact)
{
    d->contact = contact;
}

QString Message::title() const
{
    return d->title;
}

void Message::setTitle(const QString &title)
{
    d->title = title;
}

QString Message::body() const
{
    return d->body;
}

void Message::setBody(const QString &body)
{
    d->body = body;
}

Message::ReadState Message::readState() const
{
    return d->readState;
}

void Message::setReadState(Message::ReadState state)
{
    d->readState = state;
}

Message::Direction Message::direction() const
{
    return d->direction;
}

void Message::setDirection(Message::Direction direction)
{
    d->direction = direction;
}

} // namespace vk
