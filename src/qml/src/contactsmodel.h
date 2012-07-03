#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>
#include <roster.h>
#include <QWeakPointer>

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(vk::Roster *roster READ roster WRITE setRoster NOTIFY rosterChanged)
    Q_PROPERTY(QString filterByName READ filterByName WRITE setFilterByName NOTIFY filterByNameChanged)
public:
    enum Roles {
        ContactRole = Qt::UserRole + 1,
        PhotoRole
    };

    explicit ContactsModel(QObject *parent = 0);
    
    void setRoster(vk::Roster *roster);
    vk::Roster *roster() const;
    int count() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
    void setFilterByName(const QString &filter);
    QString filterByName();
    void clear();
public slots:
    int findContact(int id) const;
signals:
    void rosterChanged(vk::Roster*);
    void filterByNameChanged(const QString &filter);
    void requestFinished();
private slots:
    void addFriend(vk::Buddy *);
    void removeFriend(int id);
protected:
    bool checkContact(vk::Contact*);
private:
    QWeakPointer<vk::Roster> m_roster;
    vk::ContactList m_contactList;
    bool m_showGroups;
    QString m_filterByName;
};

#endif // CONTACTSMODEL_H
