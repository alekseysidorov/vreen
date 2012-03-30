#include "messagemodel.h"
#include "buddy.h"

namespace vk {

class MessageListModel;
class MessageListModelPrivate
{
    Q_DECLARE_PUBLIC(MessageListModel)
public:
    MessageListModelPrivate(MessageListModel *q) : q_ptr(q) {}
    MessageListModel *q_ptr;

    MessageList messageList;
};

MessageListModel::MessageListModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new MessageListModelPrivate(this))
{
    auto roles = roleNames();
    roles[TitleRole] = "title";
    roles[BodyRole] = "body";
    roles[ContactRole] = "contact";
    roles[ReadStateRole] = "readState";
    roles[DirectionRole] = "direction";
    setRoleNames(roles);
}

MessageListModel::~MessageListModel()
{

}

int MessageListModel::count() const
{
    return d_func()->messageList.count();
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    Q_D(const MessageListModel);
    int row = index.row();
    auto message = d->messageList.at(row);
    switch (role) {
    case TitleRole:
        return message.title();
        break;
    case BodyRole:
        return message.body();
    case ContactRole:
        return qVariantFromValue(message.contact());
    case ReadStateRole:
        return message.readState();
    case DirectionRole:
        return message.direction();
    default:
        break;
    }
    return QVariant::Invalid;
}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

void MessageListModel::addMessage(const Message &message)
{
    Q_D(MessageListModel);
    beginInsertRows(QModelIndex(), d->messageList.count(), d->messageList.count());
    d->messageList.append(message);
    endInsertRows();
}

void MessageListModel::removeMessage(const Message &message)
{
    Q_D(MessageListModel);
    int index = d->messageList.indexOf(message);
    if (index == -1)
        return;
    beginRemoveRows(QModelIndex(), index, index);
    d->messageList.removeAt(index);
    endRemoveRows();
}

void MessageListModel::setMessages(const MessageList &messages)
{
    clear();
    foreach (auto message, messages)
        addMessage(message);
}

void MessageListModel::clear()
{
    Q_D(MessageListModel);
    beginRemoveRows(QModelIndex(), 0, d->messageList.count());
    setMessages(MessageList());
    endRemoveRows();
}

} //namespace vk

#include "moc_messagemodel.cpp"
