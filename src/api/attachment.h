#ifndef VK_ATTACHMENT_H
#define VK_ATTACHMENT_H

#include <QSharedDataPointer>
#include <QVariantMap>
#include "vk_global.h"

namespace vk {

class AttachmentData;

class VK_SHARED_EXPORT Attachment
{
    Q_GADGET
    Q_ENUMS(Type)
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
        Page,
        Other = -1
    };

    Attachment();
    Attachment(const Attachment &);
    Attachment &operator=(const Attachment &);
    ~Attachment();

    void setData(const QVariantMap &data);
    QVariantMap data() const;
    Type type() const;

    static Attachment fromData(const QVariant &data);
    static QList<Attachment> fromVariantList(const QVariantList &list);
    static QVariantList toList(const QList<Attachment> &list);

    QVariant property(const QString &name, const QVariant &def = QVariant()) const;
    template<typename T>
    T property(const char *name, const T &def) const
    { return qVariantValue<T>(property(name, qVariantFromValue<T>(def))); }
    void setProperty(const QString &name, const QVariant &value);
    QStringList dynamicPropertyNames() const;
protected:
    Attachment(const QVariantMap &data);
private:
    QSharedDataPointer<AttachmentData> d;
};

typedef QList<Attachment> AttachmentList;
typedef QMultiHash<Attachment::Type, Attachment> AttachmentHash;

} // namespace vk

Q_DECLARE_METATYPE(vk::Attachment)
Q_DECLARE_METATYPE(vk::AttachmentList)
Q_DECLARE_METATYPE(vk::AttachmentHash)

#endif // VK_ATTACHMENT_H
