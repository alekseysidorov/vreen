#include "messagemodel.h"
#include "contact.h"
#include <QDateTime>
#include "longpoll.h"
#include "utils.h"
#include <QCoreApplication>

namespace vk {

static bool lessThanId(const Message &a, const Message &b)
{
    return a.id() < b.id();
}

static bool moreThanId(const Message &a, const Message &b)
{
    return a.id() > b.id();
}

class MessageListModel;
class MessageListModelPrivate
{
    Q_DECLARE_PUBLIC(MessageListModel)
public:
    MessageListModelPrivate(MessageListModel *q) : q_ptr(q),
        sortOrder(Qt::DescendingOrder) {}
    MessageListModel *q_ptr;

    MessageList messageList;
    Qt::SortOrder sortOrder;
};


MessageListModel::MessageListModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new MessageListModelPrivate(this))
{
    auto roles = roleNames();
    roles[SubjectRole] = "subject";
    roles[BodyRole] = "body";
    roles[FromRole] = "from";
    roles[ToRole] = "to";
    roles[ReadStateRole] = "unread";
    roles[DirectionRole] = "incoming";
    roles[DateRole] = "date";
    roles[IdRole] = "mid";
    roles[AttachmentRole] = "attachments";
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
    case SubjectRole:
        return message.subject();
        break;
    case BodyRole:
        return message.body();
    case FromRole:
        return qVariantFromValue(message.from());
    case ToRole:
        return qVariantFromValue(message.to());
    case ReadStateRole:
        return message.isUnread();
    case DirectionRole:
        return message.isIncoming();
    case DateRole:
        return message.date();
    case IdRole:
        return message.id();
    case AttachmentRole:
        return Attachment::toVariantMap(message.attachments());
    default:
        break;
    }
    return QVariant::Invalid;
}

int MessageListModel::rowCount(const QModelIndex &) const
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
    int index = findMessage(message.id());
    if (index != -1) {
        return;
    }

    index = lowerBound(d->messageList, message, d->sortOrder == Qt::AscendingOrder ? lessThanId
                                                                                   : moreThanId);
    doInsertMessage(index, message);
}

void MessageListModel::removeMessage(const Message &message)
{
    Q_D(MessageListModel);
    int index = d->messageList.indexOf(message);
    if (index == -1)
        return;
    doRemoveMessage(index);
}

void MessageListModel::removeMessage(int id)
{
    int index = findMessage(id);
    if (index != -1)
        doRemoveMessage(index);
}

void MessageListModel::setMessages(const MessageList &messages)
{
    clear();
    foreach (auto message, messages) {
        addMessage(message);
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

void MessageListModel::clear()
{
    Q_D(MessageListModel);
    beginRemoveRows(QModelIndex(), 0, d->messageList.count());
    d->messageList.clear();
    endRemoveRows();
}

void MessageListModel::doReplaceMessage(int i, const Message &message)
{
    auto index = createIndex(i, 0);
    d_func()->messageList[i] = message;
    emit dataChanged(index, index);
}

void MessageListModel::doInsertMessage(int index, const Message &message)
{
    Q_D(MessageListModel);
    beginInsertRows(QModelIndex(), index, index);
    d->messageList.insert(index, message);
    endInsertRows();
}

void MessageListModel::doRemoveMessage(int index)
{
    Q_D(MessageListModel);
    beginRemoveRows(QModelIndex(), index, index);
    d->messageList.removeAt(index);
    endRemoveRows();
}

void MessageListModel::sort(int column, Qt::SortOrder order)
{
    Q_D(MessageListModel);
    Q_UNUSED(column);
    if (order == Qt::AscendingOrder)
        qStableSort(d->messageList.begin(), d->messageList.end(), lessThanId);
    else
        qStableSort(d->messageList.begin(), d->messageList.end(), moreThanId);
    emit dataChanged(createIndex(0, 0), createIndex(d->messageList.count(), 0));
}

void MessageListModel::replaceMessageFlags(int id, int mask, int userId)
{
    Q_UNUSED(userId);
    int index = findMessage(id);
    if (index == -1)
        return;

    auto message = at(index);
    Message::Flags flags = message.flags();
    flags |= static_cast<Message::Flags>(mask);
    message.setFlags(flags);
    doReplaceMessage(index, message);
}

void MessageListModel::resetMessageFlags(int id, int mask, int userId)
{
    Q_UNUSED(userId);
    int index = findMessage(id);
    if (index == -1)
        return;

    auto message = at(index);
    auto flags = message.flags();
    flags &= ~mask;
    message.setFlags(flags);
    doReplaceMessage(index, message);
}

} //namespace vk

#include "moc_messagemodel.cpp"
