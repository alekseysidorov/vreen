#include "wallpost.h"
#include "dynamicpropertydata_p.h"
#include <QDateTime>
#include "roster.h"
#include "client.h"

namespace vk {

class WallPostData : public DynamicPropertyData
{
public:
    WallPostData(Client *client) : DynamicPropertyData(),
        client(client),
        id(0),
        fromId(-1),
        toId(-1)
    {}
    WallPostData(const WallPostData &o) : DynamicPropertyData(o),
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

    void setId(const QVariant &input) { id = input.toInt(); }
    QVariant getId() const { return id; }
};


namespace CompiledProperty {
static QList<QByteArray> names = QList<QByteArray>()
        << "id";
static QList<Getter> getters   = QList<Getter>()
        << static_cast<Getter>(&WallPostData::getId);
static QList<Setter> setters   = QList<Setter>()
        << static_cast<Setter>(&WallPostData::setId);
} //namespace CompiledProperty

WallPost::WallPost(Client *client) :
    d(new WallPostData(client))
{
}

WallPost::WallPost(const QVariantMap data, Client *client) :
    d(new WallPostData(client))
{
    auto it = data.constBegin();
    for (; it != data.constEnd(); it++)
        setProperty(it.key().toLatin1(), it.value());
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

QVariant WallPost::property(const char *name, const QVariant &def) const
{
    return d->property(name, def, CompiledProperty::names, CompiledProperty::getters);
}

void WallPost::setProperty(const char *name, const QVariant &value)
{
    d->setProperty(name, value, CompiledProperty::names, CompiledProperty::setters);
}

} //namespace vk
