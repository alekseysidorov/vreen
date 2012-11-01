/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
#include "wallpost.h"
#include "dynamicpropertydata_p.h"
#include <QDateTime>
#include "roster.h"
#include "client.h"

namespace Vreen {

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
        date(o.date),
        fromId(o.fromId),
        toId(o.toId),
        likes(o.likes),
        reposts(o.reposts),
        attachmentHash(o.attachmentHash),
        data(o.data)
    {}

    Client *client;
    int id;
    QString body;
    QDateTime date;
    int fromId;
    int toId;
    QVariantMap likes;
    QVariantMap reposts;
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
    d->likes = data.take("likes").toMap();
    d->reposts = data.take("reposts").toMap();
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

Attachment::Hash WallPost::attachments() const
{
    return d->attachmentHash;
}

Attachment::List WallPost::attachments(Attachment::Type type) const
{
    return d->attachmentHash.values(type);
}

void WallPost::setAttachments(const Attachment::List &list)
{
    d->attachmentHash = Attachment::toHash(list);
}

QVariantMap WallPost::likes() const
{
    return d->likes;
}

void WallPost::setLikes(const QVariantMap &likes)
{
    d->likes = likes;
}

Contact *WallPost::from()
{
    return d->client->roster()->buddy(d->fromId);
}

Contact *WallPost::to()
{
    return d->client->roster()->buddy(d->toId);
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

QVariantMap WallPost::reposts() const
{
    return d->reposts;
}

void WallPost::setReposts(const QVariantMap &reposts)
{
    d->reposts = reposts;
}


} //namespace Vreen

