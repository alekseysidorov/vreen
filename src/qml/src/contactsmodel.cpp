#include "contactsmodel.h"
#include <contact.h>
#include <QApplication>

ContactsModel::ContactsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_showGroups(false)
{
    auto roles = roleNames();
    roles[ContactRole] = "contact";
    roles[PhotoRole] = "photo";
    setRoleNames(roles);
}

void ContactsModel::setRoster(vk::Roster *roster)
{
    if (!m_roster.isNull())
        m_roster.data()->disconnect(this);
    m_roster = roster;

    foreach (auto buddy, m_roster.data()->findChildren<vk::Buddy*>())
        addFriend(buddy);


    connect(roster, SIGNAL(friendAdded(vk::Buddy*)), SLOT(addFriend(vk::Buddy*)));
    connect(roster, SIGNAL(contactRemoved(int)), SLOT(removeFriend(int)));

    emit rosterChanged(roster);
}

vk::Roster *ContactsModel::roster() const
{
    return m_roster.data();
}

int ContactsModel::count() const
{
    return m_contactList.count();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case ContactRole: {
        auto contact = m_contactList.at(row);
        return qVariantFromValue(contact);
        break;
    }
    case PhotoRole: {
        auto contact = m_contactList.at(row);
        return contact->photoSource(vk::Contact::PhotoSizeMediumRec);
    } default:
        break;
    }
    return QVariant();
}

int ContactsModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent == QModelIndex());
    Q_UNUSED(parent);
    return count();
}

void ContactsModel::setFilterByName(const QString &filter)
{
    m_filterByName = filter;
    emit filterByNameChanged(filter);

    //TODO write more fast algorythm
    clear();
    foreach (auto buddy, m_roster.data()->findChildren<vk::Buddy*>())
        addFriend(buddy);
}

QString ContactsModel::filterByName()
{
    return m_filterByName;
}

void ContactsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_contactList.count());
    m_contactList.clear();
    endRemoveRows();
}
int ContactsModel::findContact(int id) const
{
    for (int i = 0; i != m_contactList.count(); i++)
        if (m_contactList.at(i)->id() == id)
            return i;
    return -1;
}

void ContactsModel::addFriend(vk::Buddy *contact)
{
    if (!checkContact(contact))
        return;
    int index = m_contactList.count();
    beginInsertRows(QModelIndex(), index, index);
    m_contactList.append(contact);
    endInsertRows();
    //qApp->processEvents();
}

void ContactsModel::removeFriend(int id)
{
    int index = findContact(id);
    if (index == -1)
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_contactList.removeAt(index);
    endRemoveRows();
}

bool ContactsModel::checkContact(vk::Contact *contact)
{
    if (contact->type() != vk::Contact::FriendType)
        return false;
    if (!m_filterByName.isEmpty())
        return contact->name().contains(m_filterByName, Qt::CaseInsensitive);
    return true;
}
