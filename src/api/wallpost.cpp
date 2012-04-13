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
        toId(o.toId)
    {}

    Client *client;
    int id;
    QString body;
    QDateTime date;
    int fromId;
    int toId;
};

WallPost::WallPost(Client *client) :
    d(new WallPostData(client))
{
}

WallPost::WallPost(const QVariantMap data, Client *client) :
    d(new WallPostData(client))
{
    d->id = data.value("id").toInt();
    d->body = data.value("text").toString();
    d->fromId = data.value("from_id").toInt();
    d->toId = data.value("to_id").toInt();
    d->date = QDateTime::fromTime_t(data.value("date").toUInt());
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

Contact *WallPost::from()
{
    return d->client->roster()->contact(d->fromId);
}

Contact *WallPost::to()
{
    return d->client->roster()->contact(d->toId);
}

} //namespace vk
