/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
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
** $VREEN_END_LICENSE$
**
****************************************************************************/
#include "clientimpl.h"
#include <QSettings>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QThread>
#include <QTextDocument>
#include <message.h>
#include <roster.h>
#include <longpoll.h>
#include <utils.h>

Client::Client(QObject *parent) :
    Vreen::Client(parent)
{
    connect(this, SIGNAL(onlineStateChanged(bool)), this,
            SLOT(onOnlineStateChanged(bool)));

    QSettings settings;
    settings.beginGroup("connection");
    setLogin(settings.value("login").toString());
    setPassword(settings.value("password").toString());
    settings.endGroup();

    auto manager = new QNetworkConfigurationManager(this);
    connect(manager, SIGNAL(onlineStateChanged(bool)), this, SLOT(setOnline(bool)));

    connect(longPoll(), SIGNAL(messageAdded(Vreen::Message)), SLOT(onMessageAdded(Vreen::Message)));
    connect(this, SIGNAL(replyCreated(Vreen::Reply*)), SLOT(onReplyCreated(Vreen::Reply*)));
    connect(this, SIGNAL(error(Vreen::Client::Error)), SLOT(onReplyError(Vreen::Client::Error)));
}

QObject *Client::request(const QString &method, const QVariantMap &args)
{
    return Vreen::Client::request(method, args);
}

Vreen::Contact *Client::contact(int id)
{
    return roster()->buddy(id);
}

void Client::onOnlineStateChanged(bool isOnline)
{
    if (isOnline) {
        //save settings (TODO use crypto backend as possible)
        QSettings settings;
        settings.beginGroup("connection");
        settings.setValue("login", login());
        settings.setValue("password", password());
        settings.endGroup();
    }
}

void Client::setOnline(bool set)
{
    set ? connectToHost()
        : disconnectFromHost();
}

void Client::onMessageAdded(const Vreen::Message &msg)
{
    if (msg.isIncoming() && msg.isUnread())
		emit messageReceived(contact(msg.fromId()));
}

void Client::onReplyCreated(Vreen::Reply *reply)
{
    qDebug() << "--SendReply:" << reply->networkReply()->url();
    connect(reply, SIGNAL(resultReady(QVariant)),SLOT(onReplyFinished(QVariant)));
}

void Client::onReplyFinished(const QVariant &)
{
    Vreen::Reply *reply = Vreen::sender_cast<Vreen::Reply*>(sender());
    qDebug() << "--Reply finished" << reply->networkReply()->url().encodedPath();
    //qDebug() << "--data" << reply->response();
}

void Client::onReplyError(Error error)
{
    qDebug() << "--Error" <<  error;
}
