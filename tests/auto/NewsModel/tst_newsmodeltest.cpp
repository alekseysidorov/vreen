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
#include <newsmodel.h>

static QString getVariable(const char *name)
{
    return qgetenv(name);
}

class NewsModelTest : public QObject
{
    Q_OBJECT
    
public:
    NewsModelTest();
    
private Q_SLOTS:
    void testUpdate();
    void testUpdate_data();
};

NewsModelTest::NewsModelTest()
{
}

void NewsModelTest::testUpdate()
{
    QFETCH(QString, login);
    QFETCH(QString, password);

    vk::Client client(login, password);

    QEventLoop loop;
    connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit()));
    connect(&client, SIGNAL(error(vk::Client::Error)), &loop, SLOT(quit()));
    client.connectToHost();
    loop.exec();

    if (!client.isOnline())
        QFAIL("Client is offline!");

    vk::NewsModel model(&client);
    connect(&model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), &loop, SLOT(quit()));
    model.update();
    loop.exec();

    QVERIFY(model.count() > 0);
}

void NewsModelTest::testUpdate_data()
{
    QTest::addColumn<QString>("login");
    QTest::addColumn<QString>("password");

    QTest::newRow("From enviroment variables VK_LOGIN and VK_PASSWORD")
            << getVariable("VK_LOGIN")
            << getVariable("VK_PASSWORD");
}

QTEST_MAIN(NewsModelTest)

#include "tst_newsmodeltest.moc"

