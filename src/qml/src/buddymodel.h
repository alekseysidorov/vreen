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
#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>
#include <roster.h>
#include <QPointer>

class BuddyModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Vreen::Roster *roster READ roster WRITE setRoster NOTIFY rosterChanged)
    Q_PROPERTY(QString filterByName READ filterByName WRITE setFilterByName NOTIFY filterByNameChanged)
public:
    enum Roles {
        ContactRole = Qt::UserRole + 1,
        StatusStringRole,
        NameRole,
        ActivityRole,
        PhotoRole
    };

    explicit BuddyModel(QObject *parent = 0);
    
    void setRoster(Vreen::Roster *roster);
    Vreen::Roster *roster() const;
    int count() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
    void setFilterByName(const QString &filter);
    QString filterByName();
    void clear();
public slots:
    int findContact(int id) const;
signals:
    void rosterChanged(Vreen::Roster*);
    void filterByNameChanged(const QString &filter);
    void requestFinished();
private slots:
    void addFriend(Vreen::Buddy *);
    void removeFriend(int id);
protected:
    bool checkContact(Vreen::Buddy *);
private:
    QPointer<Vreen::Roster> m_roster;
    Vreen::BuddyList m_buddyList;
    bool m_showGroups;
    QString m_filterByName;
};

#endif // CONTACTSMODEL_H

