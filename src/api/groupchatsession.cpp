#include "groupchatsession.h"
#include "messagesession_p.h"
#include "client.h"
#include "roster.h"

namespace vk {

class GroupChatSession;
class GroupChatSessionPrivate : public MessageSessionPrivate
{
    Q_DECLARE_PUBLIC(GroupChatSession)
public:
    GroupChatSessionPrivate(MessageSession *q, Client *client, int uid) :
        MessageSessionPrivate(q, client, uid),
        adminId(0)
    {}
    QString title;
    ContactList contacts;
    int adminId;

    void addContact(int id);
    void removeContact(int id);

    void _q_history_received(const QVariant &response);
    void _q_message_sent(const QVariant &response);
    void _q_info_received(const QVariant &response);
    void _q_participant_added(const QVariant &response);
    void _q_participant_removed(const QVariant &response);
    void _q_title_updated(const QVariant &response);
	void _q_online_changed(bool set);
};

GroupChatSession::GroupChatSession(int chatId, Client *client) :
    MessageSession(new GroupChatSessionPrivate(this, client, chatId))
{
	connect(client, SIGNAL(onlineStateChanged(bool)), SLOT(_q_online_changed(bool)));
}

ContactList GroupChatSession::participants() const
{
    return d_func()->contacts;
}

QString GroupChatSession::title() const
{
    return d_func()->title;
}

Reply *GroupChatSession::sendMessage(const QString &body, const QString &subject, const Attachment &attachment)
{
    Q_D(GroupChatSession);
    QVariantMap args;
    //TODO move to client
    args.insert("chat_id", d->uid);
    args.insert("message", body);
    args.insert("title", subject);

	return d->client->request("messages.send", args);
}

Reply *GroupChatSession::create(Client *client, const IdList &uids, const QString &title)
{
	QStringList list;
	foreach (auto &uid, uids)
		list.append(QString::number(uid));
	QVariantMap args;
	args.insert("uids", list.join(","));
	args.insert("title", title);
	return client->request("messages.createChat", args);
}

void GroupChatSession::setTitle(const QString &title)
{
    Q_D(GroupChatSession);
    if (d->title != title) {
        d->title = title;
        emit titleChanged(title);
    }
}

Reply *GroupChatSession::getHistory(int count, int offset)
{
    Q_D(GroupChatSession);
    QVariantMap args;
    args.insert("count", count);
    args.insert("offset", offset);
    args.insert("chat_id", d->uid);

    auto reply = d->client->request("messages.getHistory", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_history_received(QVariant)));
    return reply;
}

Reply *GroupChatSession::getInfo()
{
    Q_D(GroupChatSession);
    QVariantMap args;
    args.insert("chat_id", d->uid);

    auto reply = d->client->request("messages.getChat", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_info_received(QVariant)));
    return reply;
}

Reply *GroupChatSession::addParticipant(Contact *buddy)
{
    Q_D(GroupChatSession);
    QVariantMap args;
    args.insert("chat_id", d->uid);
    args.insert("uid", buddy->id());

    auto reply = d->client->request("messages.addChatUser", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_participant_added(QVariant)));
    return reply;
}

Reply *GroupChatSession::removeParticipant(Contact *buddy)
{
    Q_D(GroupChatSession);
    QVariantMap args;
    args.insert("chat_id", d->uid);
    args.insert("uid", buddy->id());

    auto reply = d->client->request("messages.removeChatUser", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_participant_removed(QVariant)));
    return reply;
}

Reply *GroupChatSession::updateTitle(const QString &title)
{
    Q_D(GroupChatSession);
    QVariantMap args;
    args.insert("chat_id", d->uid);
    args.insert("title", title);

    auto reply = d->client->request("messages.editChat", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_title_updated(QVariant)));
    return reply;
}

void GroupChatSessionPrivate::_q_history_received(const QVariant &response)
{
    auto list = response.toList();
    Q_UNUSED(list.takeFirst());
    foreach (auto item, list) {
        QVariantMap map = item.toMap();
        Message message(map, client);
        emit q_func()->messageAdded(message);
    }
}

void GroupChatSessionPrivate::_q_info_received(const QVariant &response)
{
    Q_Q(GroupChatSession);
    auto map = response.toMap();
    adminId = map.value("admin_id").toInt();
    q->setTitle(map.value("title").toString());
    foreach (auto item, map.value("users").toList())
        addContact(item.toInt());
}

void GroupChatSessionPrivate::_q_participant_added(const QVariant &response)
{
    auto map = response.toMap();
    int id = map.value("uid").toInt();
    addContact(id);
}

void GroupChatSessionPrivate::_q_participant_removed(const QVariant &response)
{
    auto map = response.toMap();
    int id = map.value("uid").toInt();
    removeContact(id);
}

void GroupChatSessionPrivate::_q_title_updated(const QVariant &response)
{
    Q_Q(GroupChatSession);
    auto map = response.toMap();
	q->setTitle(map.value("title").toString());
}

void GroupChatSessionPrivate::_q_online_changed(bool set)
{
	foreach (auto contact, contacts) {
		if (auto buddy = qobject_cast<Buddy*>(contact)) {
			if (set)
				buddy->update();
			else
				buddy->setOnline(false);
		}
	}
}

void GroupChatSessionPrivate::addContact(int id)
{
    Q_Q(GroupChatSession);
    if (id) {
        auto contact = client->contact(id);
        if (!contacts.contains(contact)) {
            contacts.append(contact);
            emit q->participantAdded(contact);
        }
    }
}

void GroupChatSessionPrivate::removeContact(int id)
{
    Q_Q(GroupChatSession);
    if (id) {
        auto contact = client->contact(id);
        if (contacts.contains(contact)) {
            contacts.removeAll(contact);
            emit q->participantRemoved(contact);
        }
    }
}

} // namespace vk

#include "moc_groupchatsession.cpp"
