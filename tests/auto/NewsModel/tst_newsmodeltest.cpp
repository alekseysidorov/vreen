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
