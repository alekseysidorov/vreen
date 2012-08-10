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
#include <QString>
#include <QtTest>
#include <longpoll.h>
#include <client.h>

static QString getVariable(const char *name)
{
    return qgetenv(name);
}


class LongPollTest : public QObject
{
    Q_OBJECT
private slots:
    void testUpdates_data()
    {
        QTest::addColumn<QString>("login");
        QTest::addColumn<QString>("password");

        QTest::newRow("From enviroment variables VK_LOGIN and VK_PASSWORD")
                << getVariable("VK_LOGIN")
                << getVariable("VK_PASSWORD");
    }

    void testUpdates()
    {
        QFETCH(QString, login);
        QFETCH(QString, password);

        Vreen::Client client(login, password);

        QEventLoop loop;
        connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit()));
        connect(&client, SIGNAL(error(Vreen::Client::Error)), &loop, SLOT(quit()));
        client.connectToHost();
        loop.exec();

        if (!client.isOnline())
            QFAIL("Client is offline!");

        Vreen::LongPoll poll(&client);
        loop.exec();
    }
};

QTEST_MAIN(LongPollTest)

#include "tst_longpolltest.moc"

