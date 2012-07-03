#include "dialogsmodel.h"
#include <client.h>
#include <longpoll.h>
#include <QDebug>
#include <QApplication>

DialogsModel::DialogsModel(QObject *parent) :
    vk::MessageListModel(parent),
    m_unreadCount(0)
{
    setSortOrder(Qt::DescendingOrder);
}

void DialogsModel::setClient(QObject *client)
{
    if (m_client.data())
        m_client.data()->longPoll()->disconnect(this);

    m_client = static_cast<decltype(m_client.data())>(client);

    auto longPoll = m_client.data()->longPoll();
    connect(longPoll, SIGNAL(messageAdded(const vk::Message)), SLOT(onAddMessage(vk::Message)));
    connect(longPoll, SIGNAL(messageFlagsReplaced(int, int, int)), SLOT(replaceMessageFlags(int, int, int)));
    connect(longPoll, SIGNAL(messageFlagsReseted(int,int,int)), SLOT(resetMessageFlags(int,int)));
    emit clientChanged(m_client.data());
}

QObject *DialogsModel::client() const
{
    return m_client.data();
}

void DialogsModel::setUnreadCount(int count)
{
    m_unreadCount = count;
    emit unreadCountChanged(count);
}

//void DialogsModel::setClient(vk::Client *client)
//{
//    m_client = client;
//    emit clientChanged(client);
//}

//vk::Client *DialogsModel::client() const
//{
//    return m_client.data();
//}

void DialogsModel::getLastDialogs(int count, int previewLength)
{
    if (m_client.isNull()) {
        qWarning("Dialog model must have a client!");
        return;
    }
    auto reply = m_client.data()->getLastDialogs(count, previewLength);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(onDialogsReceived(QVariant)));
    connect(reply, SIGNAL(resultReady(QVariant)), SIGNAL(requestFinished()));
}


void DialogsModel::onDialogsReceived(const QVariant &dialogs)
{
    auto list = dialogs.toList();
    Q_UNUSED(list.takeFirst().toInt());
    vk::MessageList messageList;
    foreach (auto item, list) {
        vk::Message message(item.toMap(), m_client.data());
        messageList.append(message);
    }

    foreach (auto message, messageList) {
        onAddMessage(message);
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

static vk::Contact *getContact(const vk::Message &message)
{
    return message.isIncoming() ? message.from()
                                : message.to();
}

void DialogsModel::onAddMessage(const vk::Message &message)
{
    //FIXME use declarative style
    for (int i = 0; i != count(); i++) {
        auto old = at(i);
        if (getContact(message) == getContact(old)) {
            if (old.id() != message.id())
                removeMessage(at(i));
            break;
        }
    }
    addMessage(message);
}

int DialogsModel::unreadCount() const
{
    return m_unreadCount;
}


void DialogsModel::doReplaceMessage(int index, const vk::Message &message)
{
    if (message.isIncoming()) {
        if (at(index).isUnread() && !message.isUnread()) {
            m_unreadCount--;
            emit unreadCountChanged(m_unreadCount);
        } else if (!at(index).isUnread() && message.isUnread()) {
            m_unreadCount++;
            emit unreadCountChanged(m_unreadCount);
        }
    }
    MessageListModel::doReplaceMessage(index, message);
}

void DialogsModel::doInsertMessage(int index, const vk::Message &message)
{
    if (message.isIncoming() && message.isUnread()) {
        m_unreadCount++;
        emit unreadCountChanged(m_unreadCount);
    }
    MessageListModel::doInsertMessage(index, message);
}

void DialogsModel::doRemoveMessage(int index)
{
    auto message = at(index);
    if (message.isIncoming() && message.isUnread()) {
        m_unreadCount--;
        emit unreadCountChanged(m_unreadCount);
    }
    MessageListModel::doRemoveMessage(index);
}
