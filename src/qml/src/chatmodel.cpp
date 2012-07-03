#include "chatmodel.h"
#include <chatsession.h>
#include "contact.h"
#include "client.h"
#include "longpoll.h"

ChatModel::ChatModel(QObject *parent) :
    vk::MessageListModel(parent)
{
    setSortOrder(Qt::AscendingOrder);
}

void ChatModel::setContact(vk::Contact *contact)
{
    if (!m_session.isNull()) {
        clear();
        m_session.data()->deleteLater();
    }
    if (!contact)
        return;
    auto session = new vk::ChatSession(contact);
    auto longPoll = contact->client()->longPoll();
    connect(session, SIGNAL(messageAdded(vk::Message)), SLOT(addMessage(vk::Message)));
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

vk::Contact *ChatModel::contact() const
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
           vk::IdList ids;
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
