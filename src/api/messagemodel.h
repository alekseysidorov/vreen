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
public:
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        BodyRole,
        FromRole,
        ReadStateRole,
        DirectionRole,
        DateRole
    };

    MessageListModel(QObject *parent = 0);
    virtual ~MessageListModel();
	int count() const;
	Message at(int index) const;
    int findMessage(int id);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
public slots:
    void addMessage(const vk::Message &message);
    void removeMessage(const vk::Message &message);
    void removeMessage(int id);
    void setMessages(const vk::MessageList &messages);
    void clear();
protected:
	void replaceMessage(int index, const::vk::Message &message);
    void insertMessage(int index, const::vk::Message &message);
private:
    QScopedPointer<MessageListModelPrivate> d_ptr;
};

} //namespace vk

#endif // MESSAGEMODEL_H
