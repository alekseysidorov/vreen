#include "attachment.h"
#include <QSharedData>
#include <QStringList>

namespace vk {

static QStringList types = QStringList()
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

    static Attachment::Type getType(const QVariantMap &data)
    {
        return static_cast<Attachment::Type>(types.indexOf(data.value("type").toString()));
    }
};

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
    d->data = data;
    d->data.insert("type", d->type = AttachmentData::getType(data));
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

QList<Attachment> Attachment::fromVariantList(const QVariantList &list)
{
    AttachmentList attachments;
    foreach (auto item, list)
        attachments.append(Attachment::fromData(item.toMap()));
    return attachments;
}

QVariantList Attachment::toList(const QList<Attachment> &list)
{
    QVariantList variantList;
    foreach (auto item, list)
        variantList.append(item.data());
    return variantList;
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
