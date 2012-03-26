#ifndef VK_ROSTER_H
#define VK_ROSTER_H

#include "vk_global.h"
#include <QVariant>

namespace vk {

class Contact;
class RosterPrivate;
class VK_SHARED_EXPORT Roster : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Roster)
public:
    Roster(Client *client);
    virtual ~Roster();
    Contact *contact(int id) const;
    Contact *contact(const QVariantMap &data);
public slots:
    void sync(const QStringList &filter = QStringList());
signals:
    void contactAdded(Contact *contact);
    void contactRemoved(Contact *contact);
protected:
    QScopedPointer<RosterPrivate> d_ptr;

    //friend class Contact;
    //friend class Buddy;
    //friend class Group;

    //Q_PRIVATE_SLOT(d_func(), void _q_)
};

} // namespace vk

#endif // VK_ROSTER_H
