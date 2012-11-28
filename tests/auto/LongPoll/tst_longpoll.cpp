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
#include <longpoll.h>

#include "common/utils.h"

class LongPollTest : public QObject
{
    Q_OBJECT
public:
    LongPollTest()
    {
        VREEN_TEST_PREPARE();
    }
private slots:
    void testUpdates_data()
    {
        VREEN_ADD_LOGIN_VARS();
    }

    void testUpdates()
    {
        VREEN_CREATE_CLIENT();

        client.longPoll()->setRunning(true);
        connect(client.longPoll(), SIGNAL(contactStatusChanged(int,Vreen::Contact::Status)), &loop, SLOT(quit()));
        loop.exec();
    }
};

QTEST_MAIN(LongPollTest)

#include "tst_longpoll.moc"

