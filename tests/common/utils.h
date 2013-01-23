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
#ifndef UTILS_H
#define UTILS_H
#include "client.h"
#include "oauthconnection.h"
#include <QTest>
#include <QCoreApplication>

inline QString getVariable(const char *name)
{
    return qgetenv(name);
}

#define VREEN_TEST_PREPARE() \
    qApp->setApplicationName("test"); \
    qApp->setOrganizationName("vreen"); \

#define VREEN_ADD_LOGIN_VARS() \
    QTest::addColumn<QString>("login"); \
    QTest::addColumn<QString>("password"); \
    QTest::newRow("Env") \
    << getVariable("VREEN_LOGIN") \
    << getVariable("VREEN_PASSWORD");

//TODO add oauth connection support

#define VREEN_CREATE_CLIENT() \
    QFETCH(QString, login); \
    QFETCH(QString, password); \
    Vreen::Client client(login, password); \
    auto connection = new Vreen::OAuthConnection(3220807, this); \
    connection->setConnectionOption(Vreen::Connection::ShowAuthDialog, true); \
    connection->setConnectionOption(Vreen::Connection::KeepAuthData, true); \
    client.setConnection(connection); \
    QEventLoop loop; \
    connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit())); \
    connect(&client, SIGNAL(error(Vreen::Client::Error)), &loop, SLOT(quit())); \
    client.connectToHost(); \
    if (!client.isOnline()) \
    loop.exec();

#endif // UTILS_H

