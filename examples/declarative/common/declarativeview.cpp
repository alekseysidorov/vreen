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
#include "declarativeview.h"
#include <QDeclarativeContext>

DeclarativeView::DeclarativeView(QWidget *parent) :
    QDeclarativeView(parent)
{
    //auto client = new Vreen::Client(this);
    //auto con = new Vreen::OAuthConnection(1865463, client); //qutIM id
    //con->setConnectionOption(Vreen::Connection::ShowAuthDialog, true);
    //con->setDisplayType(Vreen::OAuthConnection::Popup);
    //client->setConnection(con);
    //rootContext()->setContextProperty("client", client);

    setSource(QUrl("qml/main.qml"));
    setResizeMode(SizeRootObjectToView);
}
