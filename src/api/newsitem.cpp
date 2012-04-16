#include "newsitem.h"
#include "utils.h"
#include <QSharedData>
#include <QStringList>
#include <QDateTime>

namespace vk {

class NewsItemData : public QSharedData {
public:
    NewsItemData() : QSharedData() {}
    NewsItemData(const NewsItemData &o) : QSharedData(o),
        data(o.data),
        attachmentHash(o.attachmentHash)
    {}
    QVariantMap data;
    AttachmentHash attachmentHash;
};

const static QStringList types = QStringList()
        << "post"
        << "photo"
        << "photo_tag"
        << "friend"
        << "note";

/*!
 * \brief The NewsItem class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=newsfeed.get
 */

/*!
 * \brief NewsItem::NewsItem
 */

NewsItem::NewsItem() : d(new NewsItemData)
{
}

NewsItem::NewsItem(const QVariantMap &data) : d(new NewsItemData)
{
    setData(data);
}

NewsItem::NewsItem(const NewsItem &rhs) : d(rhs.d)
{
}

NewsItem &NewsItem::operator=(const NewsItem &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

NewsItem::~NewsItem()
{
}

NewsItem NewsItem::fromData(const QVariant &data)
{
    return NewsItem(data.toMap());
}

void NewsItem::setData(const QVariantMap &data)
{
    d->data = data;
    auto attachmentList = Attachment::fromVariantList(d->data.take("attachments").toList());
    setAttachments(attachmentList);

}

QVariantMap NewsItem::data() const
{
    return d->data;
}

AttachmentList NewsItem::attachments() const
{
    return d->attachmentHash.values();
}

AttachmentList NewsItem::attachments(Attachment::Type type) const
{
    return d->attachmentHash.values(type);
}

void NewsItem::setAttachments(const AttachmentList &attachmentList)
{
    d->attachmentHash.clear();
    foreach (auto attachment, attachmentList)
        d->attachmentHash.insert(attachment.type(), attachment);
}

NewsItem::Type NewsItem::type() const
{
    return static_cast<Type>(types.indexOf(d->data.value("type").toString()));
}

void NewsItem::setType(NewsItem::Type type)
{
    d->data.insert("type", types.value(type));
}

int NewsItem::postId() const
{
    return d->data.value("post_id").toInt();
}

void NewsItem::setPostId(int postId)
{
    d->data.insert("post_id", postId);
}

int NewsItem::sourceId() const
{
    return d->data.value("source_id").toInt();
}

void NewsItem::setSourceId(int sourceId)
{
    d->data.insert("source_id", sourceId);
}

QString NewsItem::body() const
{
    return d->data.value("text").toString();
}

void NewsItem::setBody(const QString &body)
{
    d->data.insert("body", body);
}

QDateTime NewsItem::date() const
{
    return QDateTime::fromTime_t(d->data.value("date").toInt());
}

void NewsItem::setDate(const QDateTime &date)
{
    d->data.insert("date", date.toTime_t());
}

QVariant NewsItem::property(const QString &name, const QVariant &def) const
{
    return d->data.value(name, def);
}

QStringList NewsItem::dynamicPropertyNames() const
{
    return d->data.keys();
}

void NewsItem::setProperty(const QString &name, const QVariant &value)
{
    d->data.insert(name, value);
}

} // namespace vk
