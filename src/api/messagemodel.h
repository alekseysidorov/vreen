#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include "message.h"

namespace vk {

class MessageListModelPrivate;
class VK_SHARED_EXPORT MessageListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MessageListModel)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
public:

    enum Roles {
        SubjectRole = Qt::UserRole + 1,
        BodyRole,
        FromRole,
        ToRole,
        ReadStateRole,
        DirectionRole,
        DateRole,
        IdRole
    };

    MessageListModel(QObject *parent = 0);
    virtual ~MessageListModel();
    int count() const;
	Message at(int index) const;
    int findMessage(int id);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
    void setSortOrder(Qt::SortOrder order);
    Qt::SortOrder sortOrder() const;
signals:
    void sortOrderChanged(Qt::SortOrder order);
public slots:
    void addMessage(const vk::Message &message);
    void removeMessage(const vk::Message &message);
    void removeMessage(int id);
    void setMessages(const vk::MessageList &messages);
    void clear();
protected:
    virtual void doReplaceMessage(int index, const::vk::Message &message);
    virtual void doInsertMessage(int index, const::vk::Message &message);
    virtual void doRemoveMessage(int index);
    virtual void sort(int column, Qt::SortOrder order);
protected slots:
    void replaceMessageFlags(int id, int mask, int userId = 0);
    void resetMessageFlags(int id, int mask, int userId = 0);
private:
    QScopedPointer<MessageListModelPrivate> d_ptr;
};

} //namespace vk

#endif // MESSAGEMODEL_H
