/****************************************************************************
**
** VKit - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VKIT_BEGIN_LICENSE$
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
** $VKIT_END_LICENSE$
**
****************************************************************************/
#include "dialogsmodel.h"
#include <client.h>
#include <longpoll.h>
#include <roster.h>
#include <QDebug>
#include <QApplication>

DialogsModel::DialogsModel(QObject *parent) :
    Vreen::MessageListModel(parent),
    m_unreadCount(0)
{
    setSortOrder(Qt::DescendingOrder);
}
void DialogsModel::setUnreadCount(int count)
{
    m_unreadCount = count;
    emit unreadCountChanged(count);
}

void DialogsModel::setClient(QObject *obj)
{
    auto client = qobject_cast<Vreen::Client*>(obj);
    MessageListModel::setClient(client);
    if (client) {
        connect(client->longPoll(), SIGNAL(messageAdded(const Vreen::Message)), SLOT(onAddMessage(Vreen::Message)));
    }
}

void DialogsModel::getDialogs(int offset, int count, int previewLength)
{
    if (!client()) {
        qWarning("Dialog model must have a client!");
        return;
    }
    auto reply = client()->roster()->getDialogs(offset, count, previewLength);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(onDialogsReceived(QVariant)));
    connect(reply, SIGNAL(resultReady(QVariant)), SIGNAL(requestFinished()));
}


void DialogsModel::onDialogsReceived(const QVariant &dialogs)
{
    auto list = dialogs.toList();
    if (list.isEmpty())
        return;
    Q_UNUSED(list.takeFirst().toInt());
    Vreen::MessageList messageList = Vreen::Message::fromVariantList(list, client()->id());

    foreach (auto message, messageList) {
        onAddMessage(message);
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

static int getId(const Vreen::Message &message)
{
    return message.isIncoming() ? message.fromId()
                                : message.toId();
}

void DialogsModel::onAddMessage(const Vreen::Message &message)
{
    //FIXME use declarative style
    for (int i = 0; i != count(); i++) {
        auto old = at(i);
        if (getId(message) == getId(old)) {
            if (old.id() != message.id())
                doReplaceMessage(i, message);
            break;
        }
    }
    addMessage(message);
}

int DialogsModel::unreadCount() const
{
    return m_unreadCount;
}

void DialogsModel::doReplaceMessage(int index, const Vreen::Message &message)
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

void DialogsModel::doInsertMessage(int index, const Vreen::Message &message)
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

