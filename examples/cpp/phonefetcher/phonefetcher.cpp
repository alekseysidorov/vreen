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
#include "phonefetcher.h"
#include <vreen/roster.h>
#include <QDebug>
#include <QApplication>

PhoneFetcher::PhoneFetcher(QObject *parent) :
    Vreen::Client(parent)
{
    auto auth = new Vreen::OAuthConnection(3220807, this);
    auth->setConnectionOption(Vreen::Connection::ShowAuthDialog, true);
    auth->setConnectionOption(Vreen::Connection::KeepAuthData, true);
    setConnection(auth);

    connect(this, SIGNAL(onlineStateChanged(bool)), SLOT(onOnlineChanged(bool)));
    connect(roster(), SIGNAL(syncFinished(bool)), SLOT(onSynced(bool)));
}

void PhoneFetcher::fetch()
{
    connectToHost();
}

void PhoneFetcher::onOnlineChanged(bool online)
{
    if (online) {
        roster()->sync(QStringList() << QLatin1String("first_name")
                       << QLatin1String("last_name")
                       << QLatin1String("contacts"));
    }
}

void PhoneFetcher::onSynced(bool success)
{
    if (success) {
        qDebug() << tr("-- %1 contacts recieved").arg(roster()->buddies().count());
        foreach (auto buddy, roster()->buddies()) {
            QString homeNumber = buddy->property("_q_home_phone").toString();
            QString mobileNumber = buddy->property("_q_mobile_phone").toString();
            qDebug() << tr("name: %1, home: %2, mobile: %3").arg(buddy->name())
                        .arg(homeNumber.isEmpty() ? tr("unknown") : homeNumber)
                        .arg(mobileNumber.isEmpty() ? tr("unknown") : mobileNumber);
        }
    }
    qApp->quit();
}
