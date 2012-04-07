#ifndef VK_DYNAMICPROPERTYDATA_P_H
#define VK_DYNAMICPROPERTYDATA_P_H

//from euroelessar code

#include <QSharedData>
#include <QVariant>

namespace vk {

class DynamicPropertyData;

namespace CompiledProperty
{
    typedef QVariant (DynamicPropertyData::*Getter)() const;
    typedef void (DynamicPropertyData::*Setter)(const QVariant &variant);
}

class DynamicPropertyData : public QSharedData
{
public:
    typedef CompiledProperty::Getter Getter;
    typedef CompiledProperty::Setter Setter;
    DynamicPropertyData() {}
    DynamicPropertyData(const DynamicPropertyData &o) :
            QSharedData(o), names(o.names), values(o.values) {}
    QList<QByteArray> names;
    QList<QVariant> values;

    QVariant property(const char *name, const QVariant &def, const QList<QByteArray> &names,
                      const QList<Getter> &getters) const;
    void setProperty(const char *name, const QVariant &value, const QList<QByteArray> &names,
                     const QList<Setter> &setters);
};

} // namespace vk

#endif // VK_DYNAMICPROPERTYDATA_P_H
