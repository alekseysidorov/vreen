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
#ifndef VKITQMLPLUGIN_H
#define VKITQMLPLUGIN_H

#include "clientimpl.h"
#include <roster.h>
#include <contact.h>
#include "buddymodel.h"
#include "dialogsmodel.h"
#include "chatmodel.h"
#include "wallmodel.h"
#include "commentsmodel.h"
#include "newsfeedmodel.h"
#include "audiomodel.h"
#include <longpoll.h>
#include <attachment.h>
#include <newsfeed.h>
#include <connection.h>
#include <reply.h>
#include <messagesession.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <qqmlextensionplugin.h>
#include <qqml.h>
#else
#include <QDeclarativeExtensionPlugin>
#include <QtDeclarative>
#endif

#ifdef VREEN_WITH_OAUTH
#include <vreen/auth/oauthconnection.h>
#endif

static inline void registerVreenTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Vreen.Base")); // @uri Vreen.Base

    qmlRegisterType<Client>(uri, 2, 0, "Client");
    qmlRegisterType<BuddyModel>(uri, 2, 0, "BuddyModel");
    qmlRegisterType<DialogsModel>(uri, 2, 0, "DialogsModel");
    qmlRegisterType<ChatModel>(uri, 2, 0, "ChatModel");
    qmlRegisterType<WallModel>(uri, 2, 0, "WallModel");
    qmlRegisterType<NewsFeedModel>(uri, 2, 0, "NewsFeedModel");
    qmlRegisterType<CommentsModel>(uri, 2, 0, "CommentsModel");
    qmlRegisterType<AudioModel>(uri, 2, 0, "AudioModel");
#ifdef VREEN_WITH_OAUTH
    qmlRegisterType<Vreen::OAuthConnection>(uri, 2, 0, "OAuthConnection");
#endif

    qmlRegisterUncreatableType<Vreen::Reply>(uri, 2, 0, "Reply", QObject::tr("Don't use reply directly"));
    qmlRegisterUncreatableType<Vreen::Client>(uri, 2, 0, "ClientBase", QObject::tr("Use SimpleClient instead"));
    qmlRegisterUncreatableType<Vreen::Roster>(uri, 2, 0, "Roster", QObject::tr("Use client.roster instead"));
    qmlRegisterUncreatableType<Vreen::Connection>(uri, 2, 0, "Connection", QObject::tr("Use client.connection instead"));
    qmlRegisterUncreatableType<Vreen::Contact>(uri, 2, 0, "Contact", QObject::tr("Use client.contact(id)"));
    qmlRegisterUncreatableType<Vreen::Buddy>(uri, 2, 0, "Buddy", QObject::tr("User roster.buddy(id)"));
    qmlRegisterUncreatableType<Vreen::Message>(uri, 2, 0, "Message", QObject::tr("Only flags"));
    qmlRegisterUncreatableType<Vreen::LongPoll>(uri, 2, 0, "LongPoll", QObject::tr("Use client.longPoll instead"));
    qmlRegisterUncreatableType<Vreen::Attachment>(uri, 2, 0, "Attachment", QObject::tr("Attachment enums"));
    qmlRegisterUncreatableType<Vreen::NewsItem>(uri, 2, 0, "NewsItem", QObject::tr("NewsItem enums"));
    qmlRegisterUncreatableType<Vreen::NewsFeed>(uri, 2, 0, "NewsFeed", QObject::tr("NewsFeed enums"));
    qmlRegisterUncreatableType<Vreen::MessageListModel>(uri, 2, 0, "MessageListModel", QObject::tr("Cannot use this class directly"));
    qmlRegisterUncreatableType<Vreen::MessageSession>(uri, 2, 0, "MessageSession", QObject::tr("Cannot use this class directly"));
}

#endif // VKITQMLPLUGIN_H

