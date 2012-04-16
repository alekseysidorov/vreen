#include "wallpost.h"
#include "dynamicpropertydata_p.h"
#include <QDateTime>
#include "roster.h"
#include "client.h"

namespace vk {

class WallPostData : public QSharedData
{
public:
    WallPostData(Client *client) : QSharedData(),
        client(client),
        id(0),
        fromId(-1),
        toId(-1)
    {}
    WallPostData(const WallPostData &o) : QSharedData(o),
        client(o.client),
        id(o.id),
        body(o.body),
        fromId(o.fromId),
        toId(o.toId),
        attachmentHash(o.attachmentHash),
        data(o.data)
    {}

    Client *client;
    int id;
    QString body;
    QDateTime date;
    int fromId;
    int toId;
    Attachment::Hash attachmentHash;
    QVariantMap data;
};

WallPost::WallPost(Client *client) :
    d(new WallPostData(client))
{
}

WallPost::WallPost(QVariantMap data, Client *client) :
    d(new WallPostData(client))
{
    d->id = data.take("id").toInt();
    d->body = data.take("text").toString();
    d->fromId = data.take("from_id").toInt();
    d->toId = data.take("to_id").toInt();
    d->date = QDateTime::fromTime_t(data.take("date").toUInt());
    setAttachments(Attachment::fromVariantList(data.take("attachments").toList()));
    d->data = data;
}

WallPost::WallPost(const WallPost &other) : d(other.d)
{
}

WallPost &WallPost::operator=(const WallPost &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

WallPost::~WallPost()
{
}

Client *WallPost::client() const
{
    return d->client;
}

void WallPost::setId(int id)
{
    d->id = id;
}

int WallPost::id() const
{
    return d->id;
}

void WallPost::setBody(const QString &body)
{
    d->body = body;
}

QString WallPost::body() const
{
    return d->body;
}

void WallPost::setFromId(int id)
{
    d->fromId = id;
}

int WallPost::fromId() const
{
    return d->fromId;
}

void WallPost::setToId(int id)
{
    d->toId = id;
}

int WallPost::toId() const
{
    return d->toId;
}

void WallPost::setDate(const QDateTime &date)
{
    d->date = date;
}

QDateTime WallPost::date() const
{
    return d->date;
}

Attachment::List WallPost::attachments() const
{
    return d->attachmentHash.values();
}

Attachment::List WallPost::attachments(Attachment::Type type) const
{
    return d->attachmentHash.values(type);
}

void WallPost::setAttachments(const Attachment::List &list)
{
    d->attachmentHash = Attachment::toHash(list);
}

Contact *WallPost::from()
{
    return d->client->roster()->contact(d->fromId);
}

Contact *WallPost::to()
{
    return d->client->roster()->contact(d->toId);
}

WallPost WallPost::fromData(const QVariant data, Client *client)
{
    return WallPost(data.toMap(), client);
}

QVariant WallPost::property(const QString &name, const QVariant &def) const
{
    return d->data.value(name, def);
}

void WallPost::setProperty(const QString &name, const QVariant &value)
{
    d->data.insert(name, value);
}

QStringList WallPost::dynamicPropertyNames() const
{
    return d->data.keys();
}

} //namespace vk
