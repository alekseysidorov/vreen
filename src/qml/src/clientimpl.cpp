#include "clientimpl.h"
#include <QSettings>
#include <QNetworkConfigurationManager>
#include <QThread>
#include <QTextDocument>
#include <message.h>
#include <roster.h>
#include <longpoll.h>
#include <utils.h>

Client::Client(QObject *parent) :
    vk::Client(parent)
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

    connect(longPoll(), SIGNAL(messageAdded(vk::Message)), SLOT(onMessageAdded(vk::Message)));
    connect(this, SIGNAL(replyCreated(vk::Reply*)), SLOT(onReplyCreated(vk::Reply*)));
    connect(this, SIGNAL(error(vk::Reply*)), SLOT(onReplyError(vk::Reply*)));
}

QObject *Client::request(const QString &method, const QVariantMap &args)
{
    return vk::Client::request(method, args);
}

vk::Contact *Client::contact(int id)
{
    return roster()->contact(id);
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

void Client::onMessageAdded(const vk::Message &msg)
{
    if (msg.isIncoming() && msg.isUnread())
        emit messageReceived(msg.from());
}

void Client::onReplyCreated(vk::Reply *reply)
{
    qDebug() << "--SendReply:" << reply->networkReply()->url();
    connect(reply, SIGNAL(resultReady(QVariant)),SLOT(onReplyFinished(QVariant)));
}

void Client::onReplyFinished(const QVariant &)
{
    vk::Reply *reply = vk::sender_cast<vk::Reply*>(sender());
    qDebug() << "--Reply finished" << reply->networkReply()->url().encodedPath();
    //qDebug() << "--data" << reply->response();
}

void Client::onReplyError(vk::Reply *reply)
{
    qDebug() << "--Error" << reply->response();
}
