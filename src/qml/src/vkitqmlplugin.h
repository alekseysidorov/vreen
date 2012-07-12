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
#ifndef VKITQMLPLUGIN_H
#define VKITQMLPLUGIN_H

#include <QDeclarativeExtensionPlugin>

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

class VKitQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("com.vk.api"));

        qmlRegisterType<Client>("com.vk.api", 1, 0, "Client");
        qmlRegisterType<BuddyModel>("com.vk.api", 1, 0, "BuddyModel");
        qmlRegisterType<DialogsModel>("com.vk.api", 1, 0, "DialogsModel");
        qmlRegisterType<ChatModel>("com.vk.api", 1, 0, "ChatModel");
        qmlRegisterType<WallModel>("com.vk.api", 1, 0, "WallModel");
        qmlRegisterType<NewsFeedModel>("com.vk.api", 1, 0, "NewsFeedModel");
        qmlRegisterType<CommentsModel>("com.vk.api", 1, 0, "CommentsModel");
        qmlRegisterType<AudioModel>("com.vk.api", 1, 0, "AudioModel");

        qmlRegisterUncreatableType<vk::Roster>("com.vk.api", 1, 0, "Roster", QObject::tr("Use client.roster instead"));
        qmlRegisterUncreatableType<vk::Contact>("com.vk.api", 1, 0, "Contact", QObject::tr("User Roster"));
        qmlRegisterUncreatableType<vk::Message>("com.vk.api", 1, 0, "Message", QObject::tr("Only flags"));
        qmlRegisterUncreatableType<vk::LongPoll>("com.vk.api", 1, 0, "LongPoll", QObject::tr("Use client.longPoll instead"));
        qmlRegisterUncreatableType<vk::Attachment>("com.vk.api", 1, 0, "Attachment", QObject::tr("Attachment enums"));
        qmlRegisterUncreatableType<vk::NewsItem>("com.vk.api", 1, 0, "NewsItem", QObject::tr("NewsItem enums"));
        qmlRegisterUncreatableType<vk::NewsFeed>("com.vk.api", 1, 0, "NewsFeed", QObject::tr("NewsFeed enums"));
    }
};

Q_EXPORT_PLUGIN2(vkitplugin, VKitQmlPlugin)


#endif // VKITQMLPLUGIN_H

