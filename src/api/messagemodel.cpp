#include "messagemodel.h"
#include "buddy.h"
#include <QDateTime>

namespace vk {

static bool lessThanId(const Message &a, const Message &b)
{
    return a.id() < b.id();
}

class MessageListModel;
class MessageListModelPrivate
{
    Q_DECLARE_PUBLIC(MessageListModel)
public:
    MessageListModelPrivate(MessageListModel *q) : q_ptr(q),
        lessThan(&lessThanId), sortOrder(Qt::DescendingOrder) {}
    MessageListModel *q_ptr;

    MessageList messageList;
    Qt::SortOrder sortOrder;

    MessageListModel::MessageLessThan lessThan;
};


MessageListModel::MessageListModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new MessageListModelPrivate(this))
{
    auto roles = roleNames();
    roles[TitleRole] = "title";
    roles[BodyRole] = "body";
    roles[FromRole] = "from";
    roles[ReadStateRole] = "readState";
    roles[DirectionRole] = "direction";
    roles[DateRole] = "date";
    setRoleNames(roles);
}

MessageListModel::~MessageListModel()
{

}

int MessageListModel::count() const
{
    return d_func()->messageList.count();
}

Message MessageListModel::at(int index) const
{
    return d_func()->messageList.at(index);
}

int MessageListModel::findMessage(int id)
{
    Q_D(MessageListModel);
    for (int index = 0; index != d->messageList.count(); index++)
        if (d->messageList.at(index).id() == id)
            return index;
    return -1;
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
    case FromRole:
        return qVariantFromValue(message.from());
    case ReadStateRole:
        return message.readState();
    case DirectionRole:
        return message.direction();
    case DateRole:
        return message.date();
    default:
        break;
    }
    return QVariant::Invalid;
}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

void MessageListModel::setSortOrder(Qt::SortOrder order)
{
    Q_D(MessageListModel);
    if (d->sortOrder != order) {
        sort(0, order);
        d->sortOrder = order;
        emit sortOrderChanged(order);
    }
}

Qt::SortOrder MessageListModel::sortOrder() const
{
    return d_func()->sortOrder;
}

void MessageListModel::addMessage(const Message &message)
{
    Q_D(MessageListModel);
    if (findMessage(message.id()) != -1) //TODO maybe need replace?
        return;

    auto it = d->sortOrder == Qt::AscendingOrder ? qLowerBound(d->messageList.begin(),
                                                               d->messageList.end(),
                                                               message,
                                                               d->lessThan)
                                                 : qLowerBound(d->messageList.end(),
                                                               d->messageList.begin(),
                                                               message,
                                                               d->lessThan);
    int index = it - d->messageList.begin();
    insertMessage(index, message);
    //beginInsertRows(QModelIndex(), d->messageList.count(), d->messageList.count());
    //d->messageList.append(message);
    //endInsertRows();
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

void MessageListModel::removeMessage(int id)
{
    Q_D(MessageListModel);
    int index = findMessage(id);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        d->messageList.removeAt(index);
        endRemoveRows();
    }
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
    d->messageList.clear();
    endRemoveRows();
}

void MessageListModel::replaceMessage(int i, const Message &message)
{
    auto index = createIndex(i, 0);
    d_func()->messageList[i] = message;
    emit dataChanged(index, index);
}

void MessageListModel::insertMessage(int index, const Message &message)
{
    Q_D(MessageListModel);
    beginInsertRows(QModelIndex(), index, index);
    d->messageList.insert(index, message);
    endInsertRows();
}

void MessageListModel::sort(int column, Qt::SortOrder order)
{
    Q_D(MessageListModel);
    Q_UNUSED(column);
    if (order == Qt::AscendingOrder)
        qStableSort(d->messageList.begin(), d->messageList.end(), d->lessThan);
    else
        qStableSort(d->messageList.end(), d->messageList.begin(), d->lessThan);
    emit dataChanged(createIndex(0, 0), createIndex(d->messageList.count(), 0));
}

} //namespace vk

#include "moc_messagemodel.cpp"
