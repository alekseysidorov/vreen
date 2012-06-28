#ifndef CLIENT_P_H
#define CLIENT_P_H

#include "client.h"
#include "reply.h"
#include <QWeakPointer>
#include "directconnection_p.h"
#include "roster.h"
#include "reply.h"
#include "message.h"
#include "longpoll.h"
#include "utils.h"
#include <QDebug>
#include <QTimer>

namespace vk {

class ClientPrivate
{
    Q_DECLARE_PUBLIC(Client)
public:
	ClientPrivate(Client *q) : q_ptr(q), isInvisible(false)
	{
		onlineUpdater.setInterval(15000 * 60);
		onlineUpdater.setSingleShot(false);
		q->connect(&onlineUpdater, SIGNAL(timeout()), q, SLOT(_q_update_online()));
	}
    Client *q_ptr;
    QString login;
    QString password;
    QWeakPointer<Connection> connection;
	QWeakPointer<Roster> roster;
	QWeakPointer<LongPoll> longPoll;
	QString activity;
	bool isInvisible;
	QTimer onlineUpdater;

	void setOnlineUpdaterRunning(bool set);

    void _q_connection_state_changed(vk::Client::State state);
	void _q_error_received(int error);
	void _q_reply_finished(const QVariant &);
	void _q_activity_update_finished(const QVariant &);
	void _q_update_online();
    void processReply(Reply *reply);
};

} //namespace vk

#endif // CLIENT_P_H
