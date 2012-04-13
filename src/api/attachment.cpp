#include "attachment.h"
#include <QSharedData>
#include <QStringList>

namespace vk {

class AttachmentData : public QSharedData {
public:
    AttachmentData() : QSharedData() {}
    AttachmentData(const AttachmentData &o) : QSharedData(o),
        data(o.data) {}
    QVariantMap data;
};

Attachment::Attachment() : d(new AttachmentData)
{
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
}

QVariantMap Attachment::data() const
{
    return d->data;
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
