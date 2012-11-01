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
#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <client.h>
#include <connection.h>
#include <reply.h>

#include "common/utils.h"

class ConnectionTest: public QObject
{
    Q_OBJECT
private slots:
    void testDirectConnection_data()
    {
        VK_ADD_LOGIN_VARS();
    }

    void testDirectConnection()
    {
        VK_CREATE_CLIENT();

        QCOMPARE(client.isOnline(), true);
    }

    void testDirectConnectionWrongData_data()
    {
        QTest::addColumn<QString>("login");
        QTest::addColumn<QString>("password");

        QTest::newRow("Wrong data")
                << QString("Foo")
                << QString("bar");
    }

    void testDirectConnectionWrongData()
    {
        VK_CREATE_CLIENT();

        QCOMPARE(client.isOnline(), false);
    }

    void testRequest_data()
    {
        VK_ADD_LOGIN_VARS();
    }

    void testRequest()
    {
        VK_CREATE_CLIENT();

        if (!client.isOnline())
            QFAIL("Client is offline!");

		Vreen::Reply *reply = client.request("getUserSettings");
        connect(reply, SIGNAL(resultReady(QVariant)), &loop, SLOT(quit()));
        loop.exec();
        QCOMPARE(reply->response().toInt() > 6, true);
    }
};

QTEST_MAIN(ConnectionTest)

#include "tst_connectiontest.moc"

