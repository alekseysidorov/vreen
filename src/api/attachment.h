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
    typedef QList<Attachment> List;
    typedef QMultiHash<Attachment::Type, Attachment> Hash;

    Attachment();
    Attachment(const Attachment &);
    Attachment &operator=(const Attachment &);
    ~Attachment();

    void setData(const QVariantMap &data);
    QVariantMap data() const;
    Type type() const;

    static Attachment fromData(const QVariant &data);
    static List fromVariantList(const QVariantList &list);
    static QVariantList toVariantList(const List &list);
    static Hash toHash(const List &list);
    static QVariantMap toVariantMap(const Hash &hash);

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

} // namespace vk

Q_DECLARE_METATYPE(vk::Attachment)
Q_DECLARE_METATYPE(vk::Attachment::List)
Q_DECLARE_METATYPE(vk::Attachment::Hash)

#endif // VK_ATTACHMENT_H
