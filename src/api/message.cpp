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
        from(client->me()),
        to(client->me()),
        readState(Message::Unknown),
        direction(Message::Forward),
        chatId(0),
        userCount(0),
        latitude(-1),
        longitude(-1)
    {}
    MessageData(const MessageData &o) :
        QSharedData(o),
        client(o.client),
        id(o.id),
        from(o.from),
        to(o.to),
        date(o.date),
        readState(o.readState),
        subject(o.subject),
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
    QWeakPointer<Contact> from;
    QWeakPointer<Contact> to;
    QDateTime date;
    Message::ReadState readState;
    Message::Direction direction;
    QString subject;
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

        int clientId = data.value("from_id").toInt();
        if (clientId) {
            auto contact = client->roster()->contact(clientId);
            if (contact == client->me()) {
                to.clear();
                direction = Message::Out;
            } else {
                direction = Message::In;
                from = contact;
            }
        } else {
            direction = flag_helper<Message::Direction>(data.value("out"));
            clientId = data.value("uid").toInt();
            auto contact = client->roster()->contact(clientId);
            if (direction == Message::In) {
                from = contact;
                to = client->me();
            } else {
                to = contact;
                from = client->me();
            }
        }

        date = QDateTime::fromTime_t(data.value("date").toInt());
        readState = flag_helper<Message::ReadState>(data.value("read_state"));
        subject = data.value("title").toString();
        body = data.value("body").toString();
        //TODO forward messages
        //TODO attachments
        //TODO groupchats
    }
};


/*!
 * \brief The Message class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=Формат_описания_личных_сообщений */

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

void Message::setId(int id)
{
    d->id = id;
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

Contact *Message::from() const
{
    return d->from.data();
}

void Message::setFrom(Contact *contact)
{
    d->from = contact;
}

Contact *Message::to() const
{
    return d->to.data();
}

void Message::setTo(Contact *to)
{
    d->to = to;
}

QString Message::subject() const
{
    return d->subject;
}

void Message::setSubject(const QString &title)
{
    d->subject = title;
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
