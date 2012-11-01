/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
#include "buddymodel.h"
#include <contact.h>
#include <utils.h>
#include <QApplication>

#include <QDebug>

BuddyModel::BuddyModel(QObject *parent) :
    QAbstractListModel(parent),
    m_showGroups(false)
{
    auto roles = roleNames();
    roles[ContactRole] = "contact";
    roles[StatusStringRole] = "statusString";
    roles[PhotoRole] = "photo";
    roles[NameRole] = "name";
    roles[ActivityRole] = "activity";
    setRoleNames(roles);
}

void BuddyModel::setRoster(Vreen::Roster *roster)
{
    if (!m_roster.isNull())
        m_roster.data()->disconnect(this);
    m_roster = roster;

    foreach (auto buddy, m_roster.data()->buddies())
        addFriend(buddy);

	connect(roster, SIGNAL(buddyAdded(Vreen::Buddy*)), SLOT(addFriend(Vreen::Buddy*)));
	connect(roster, SIGNAL(buddyRemoved(int)), SLOT(removeFriend(int)));
    emit rosterChanged(roster);
}

Vreen::Roster *BuddyModel::roster() const
{
    return m_roster.data();
}

int BuddyModel::count() const
{
    return m_buddyList.count();
}

QVariant BuddyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case ContactRole: {
        auto contact = m_buddyList.at(row);
        return qVariantFromValue(contact);
        break;
    }
    case StatusStringRole: {
        auto buddy = m_buddyList.at(row);
        auto status = buddy ? buddy->status() : Vreen::Contact::Unknown;
        switch (status) {
        case Vreen::Contact::Online:
            return tr("Online");
        case Vreen::Contact::Offline:
            return tr("Offline");
        case Vreen::Contact::Away:
            return tr("Away");
        default:
            break;
        }
    }
    case ActivityRole: {
        return m_buddyList.at(row)->activity();
    }
    case NameRole: {
        return m_buddyList.at(row)->name();
    }
    case PhotoRole: {
        auto contact = m_buddyList.at(row);
        return contact->photoSource(Vreen::Contact::PhotoSizeMediumRec);
    } default:
        break;
    }
    return QVariant();
}

int BuddyModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent == QModelIndex());
    Q_UNUSED(parent);
    return count();
}

void BuddyModel::setFilterByName(const QString &filter)
{
    m_filterByName = filter;
    emit filterByNameChanged(filter);

    //TODO write more fast algorythm
    clear();
    foreach (auto buddy, m_roster.data()->findChildren<Vreen::Buddy*>())
        addFriend(buddy);
}

QString BuddyModel::filterByName()
{
    return m_filterByName;
}

void BuddyModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_buddyList.count());
    m_buddyList.clear();
    endRemoveRows();
}
int BuddyModel::findContact(int id) const
{
    for (int i = 0; i != m_buddyList.count(); i++)
        if (m_buddyList.at(i)->id() == id)
            return i;
    return -1;
}

//static bool buddyLessThan(const Vreen::Buddy *a, const Vreen::Buddy *b)
//{
////    if (a->status() == b->status()) {
////        return QString::compare(a->name(), b->name(), Qt::CaseInsensitive) < 0;
////    } else
//    return a->status() < b->status();
//}

void BuddyModel::addFriend(Vreen::Buddy *contact)
{
    if (!checkContact(contact))
        return;
    int index = m_buddyList.count();
    //auto index = Vreen::lowerBound(m_buddyList, contact, buddyLessThan) + 1;

    beginInsertRows(QModelIndex(), index, index);
    m_buddyList.insert(index, contact);
    endInsertRows();
    //qApp->processEvents();
}

void BuddyModel::removeFriend(int id)
{
    int index = findContact(id);
    if (index == -1)
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_buddyList.removeAt(index);
    endRemoveRows();
}

bool BuddyModel::checkContact(Vreen::Buddy *contact)
{
    if (!contact->isFriend())
        return false;
    if (!m_filterByName.isEmpty())
        return contact->name().contains(m_filterByName, Qt::CaseInsensitive);
    return true;
}

