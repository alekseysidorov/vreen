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
#include "chatmodel.h"
#include <chatsession.h>
#include "contact.h"
#include "client.h"
#include "longpoll.h"

ChatModel::ChatModel(QObject *parent) :
    Vreen::MessageListModel(parent)
{
    setSortOrder(Qt::AscendingOrder);
}

void ChatModel::setContact(Vreen::Contact *contact)
{
    if (!m_session.isNull()) {
        clear();
        m_session.data()->deleteLater();
    }
    if (!contact)
        return;
    setClient(contact->client());
    auto session = new Vreen::ChatSession(contact);
    auto longPoll = contact->client()->longPoll();
    connect(session, SIGNAL(messageAdded(Vreen::Message)), SLOT(addMessage(Vreen::Message)));
    connect(session, SIGNAL(messageDeleted(int)), SLOT(removeMessage(int)));
    connect(session, SIGNAL(messageReadStateChanged(int,bool)),
            this, SLOT(messageReadStateChanged(int,bool)));
    connect(longPoll, SIGNAL(messageFlagsReplaced(int, int, int)),
            this, SLOT(replaceMessageFlags(int, int, int)));
    connect(longPoll, SIGNAL(messageFlagsReseted(int, int, int)),
            this, SLOT(resetMessageFlags(int, int, int)));

    m_session = session;
    emit titleChanged(session->title());
}

Vreen::Contact *ChatModel::contact() const
{
    if (m_session.isNull())
        return 0;
    else
        return m_session.data()->contact();
}

QString ChatModel::title() const
{
    if (m_session.isNull())
        return tr("Contact isn't set!");
    else
        return m_session.data()->contact()->name();
}

void ChatModel::getHistory(int count, int offset)
{
    if (!m_session.isNull()) {
        auto reply = m_session.data()->getHistory(count, offset);
        connect(reply, SIGNAL(resultReady(QVariant)), SIGNAL(requestFinished()));
    }
}

void ChatModel::markAsRead(int mid, bool set)
{
       if (!m_session.isNull()) {
           Vreen::IdList ids;
           ids.append(mid);
           m_session.data()->markMessagesAsRead(ids, set);
       }
}

void ChatModel::messageReadStateChanged(int id, bool set)
{
    int index = findMessage(id);
    if (index == -1)
        return;

    auto message = at(index);
    message.setUnread(!set);
    doReplaceMessage(index, message);
}

