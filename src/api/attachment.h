#ifndef VK_ATTACHMENT_H
#define VK_ATTACHMENT_H

#include <QSharedDataPointer>
#include <QVariantMap>

namespace vk {

class AttachmentData;

class Attachment
{
public:
    enum Type {
        Photo,
        PostedPhoto,
        Video,
        Audio,
        Document,
        Graffiti,
        Link,
        Note,
        ApplicationImage,
        Poll,
        Page
    };

    Attachment();
    Attachment(const Attachment &);
    Attachment &operator=(const Attachment &);
    ~Attachment();
    void setData(const QVariantMap &data);
    QVariantMap data() const;

    QVariant property(const char *name, const QVariant &def = QVariant()) const;
    template<typename T>
    T property(const char *name, const T &def) const
    { return qVariantValue<T>(property(name, qVariantFromValue<T>(def))); }
    void setProperty(const char *name, const QVariant &value);
    QList<QByteArray> dynamicPropertyNames() const;
private:
    QSharedDataPointer<AttachmentData> d;
};

} // namespace vk

#endif // VK_ATTACHMENT_H
