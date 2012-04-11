#include "attachment.h"
#include <QSharedData>

namespace vk {

class AttachmentData : public QSharedData {
public:
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
}

QVariantMap Attachment::data() const
{
}

QVariant Attachment::property(const char *name, const QVariant &def) const
{
}

QList<QByteArray> Attachment::dynamicPropertyNames() const
{
}

void Attachment::setProperty(const char *name, const QVariant &value)
{

}

} // namespace vk
