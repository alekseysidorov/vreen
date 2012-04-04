#ifndef VK_ROSTER_H
#define VK_ROSTER_H

#include "buddy.h"
#include <QVariant>
#include <QStringList>

namespace vk {
class Client;
typedef QList<int> IdList;

class RosterPrivate;
class VK_SHARED_EXPORT Roster : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Roster)
public:
    Roster(Client *client, int uid = 0);
    virtual ~Roster();
    void setUid(int uid);
    int uid() const;

    Contact *owner() const;
    Contact *contact(int id);
    Contact *contact(int id) const;
    Contact *contact(const QVariantMap &data);
    ContactList contacts() const;

    QStringList tags() const;
    void setTags(const QStringList &list);
public slots:
    void sync(const QStringList &fields = QStringList()
            << VK_COMMON_FIELDS
            );
    void update(const IdList &ids, const QStringList &fields = QStringList()
            << VK_ALL_FIELDS
            );
signals:
    void contactAdded(vk::Contact *contact);
    void contactRemoved(vk::Contact *contact);
    void tagsChanged(const QStringList &);
    void syncFinished(bool success);
    void uidChanged(int uid);
protected:
    QScopedPointer<RosterPrivate> d_ptr;

    //friend class Contact;
    //friend class Buddy;
    //friend class Group;

    Q_PRIVATE_SLOT(d_func(), void _q_tags_received(const QVariant &response))
    Q_PRIVATE_SLOT(d_func(), void _q_friends_received(const QVariant &response))
    Q_PRIVATE_SLOT(d_func(), void _q_status_changed(int userId, vk::Buddy::Status status))
};

} // namespace vk

#endif // VK_ROSTER_H
