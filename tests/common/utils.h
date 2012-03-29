#ifndef UTILS_H
#define UTILS_H
#include "client.h"

QString getVariable(const char *name)
{
    return qgetenv(name);
}

#define VK_ADD_LOGIN_VARS() \
    QTest::addColumn<QString>("login"); \
    QTest::addColumn<QString>("password"); \
    QTest::newRow("From enviroment variables VK_LOGIN and VK_PASSWORD") \
    << getVariable("VK_LOGIN") \
    << getVariable("VK_PASSWORD");


#define VK_CREATE_CLIENT() \
    QFETCH(QString, login); \
    QFETCH(QString, password); \
    vk::Client client(login, password); \
    QEventLoop loop; \
    connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit())); \
    connect(&client, SIGNAL(error(vk::Client::Error)), &loop, SLOT(quit())); \
    client.connectToHost(); \
    loop.exec();

#endif // UTILS_H
