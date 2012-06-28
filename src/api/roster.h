/****************************************************************************
**
** VKit - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VKIT_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VKIT_END_LICENSE$
**
****************************************************************************/
#ifndef VK_ROSTER_H
#define VK_ROSTER_H

#include "contact.h"
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

    enum NameCase {
        NomCase,
        GenCase,
        DatCase,
        AccCase,
        InsCase,
        AblCase
    };

    Roster(Client *client, int uid = 0);
    virtual ~Roster();
    void setUid(int uid);
    int uid() const;

    Contact *owner() const;
    Contact *contact(int id, Contact::Type type = Contact::BuddyType);
    Contact *contact(int id) const;
    ContactList contacts() const;
    static void fillContact(Contact *contact, const QVariantMap &data);

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
    void friendAdded(vk::Buddy *contact);
    void contactRemoved(int id);
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
    Q_PRIVATE_SLOT(d_func(), void _q_status_changed(int userId, vk::Contact::Status status))
    Q_PRIVATE_SLOT(d_func(), void _q_online_changed(bool))
};

} // namespace vk

Q_DECLARE_METATYPE(vk::Roster*)

#endif // VK_ROSTER_H

