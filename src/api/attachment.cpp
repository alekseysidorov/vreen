#include "attachment.h"
#include <QSharedData>
#include <QStringList>

namespace vk {

const static QStringList types = QStringList()
        << "photo"
        << "posted_photo"
        << "video"
        << "audio"
        << "doc"
        << "graffiti"
        << "link"
        << "note"
        << "app"
        << "poll"
        << "page";

class AttachmentData : public QSharedData {
public:
    AttachmentData() : QSharedData(),
        type(Attachment::Other) {}
    AttachmentData(const AttachmentData &o) : QSharedData(o),
        data(o.data) {}
    Attachment::Type type;
    QVariantMap data;

    static Attachment::Type getType(const QString &type)
    {
        return static_cast<Attachment::Type>(types.indexOf(type));
    }
};

/*!
 * \brief The Attachment class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=Описание_поля_attachments
 */

/*!
 * \brief Attachment::Attachment
 */
Attachment::Attachment() : d(new AttachmentData)
{
}

Attachment::Attachment(const QVariantMap &data) : d(new AttachmentData)
{
    setData(data);
}

Attachment::Attachment(const Attachment &rhs) : d(rhs.d)
{
}

Attachment &Attachment::operator=(const Attachment &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

Attachment::~Attachment()
{
}

void Attachment::setData(const QVariantMap &data)
{
    d->data.clear();
    QString type = data.value("type").toString();

    //move properties to top level
    auto map = data.value(type).toMap();
    for (auto it = map.constBegin(); it != map.constEnd(); it++)
        d->data.insert(it.key(), it.value());
    //convert type to enum
    d->data.insert("type", d->type = AttachmentData::getType(type));
}

QVariantMap Attachment::data() const
{
    return d->data;
}

Attachment::Type Attachment::type() const
{
    return d->type;
}

Attachment Attachment::fromData(const QVariant &data)
{
    return Attachment(data.toMap());
}

Attachment::List Attachment::fromVariantList(const QVariantList &list)
{
    Attachment::List attachments;
    foreach (auto item, list)
        attachments.append(Attachment::fromData(item.toMap()));
    return attachments;
}

QVariantList Attachment::toVariantList(const Attachment::List &list)
{
    QVariantList variantList;
    foreach (auto item, list)
        variantList.append(item.data());
    return variantList;
}

Attachment::Hash Attachment::toHash(const Attachment::List &list)
{
    Hash hash;
    foreach (auto attachment, list)
        hash.insert(attachment.type(), attachment);
    return hash;
}

QVariant Attachment::property(const QString &name, const QVariant &def) const
{
    return d->data.value(name, def);
}

QStringList Attachment::dynamicPropertyNames() const
{
    return d->data.keys();
}

void Attachment::setProperty(const QString &name, const QVariant &value)
{
    d->data.insert(name, value);
}

} // namespace vk
