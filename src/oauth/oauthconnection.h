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
#ifndef VK_OAUTHCONNECTION_H
#define VK_OAUTHCONNECTION_H

#include <vreen/connection.h>
#include <ctime>

class QWebPage;

namespace Vreen {

class OAuthConnectionPrivate;

class OAuthConnection : public Connection
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OAuthConnection)
    Q_ENUMS(DisplayType)
    Q_PROPERTY(int appId READ appId WRITE setAppId NOTIFY appIdChanged)

public:
    enum DisplayType {
        Page,
        Popup,
        Touch,
        Wap
    };
    explicit OAuthConnection(int appId, QObject *parent = 0);
    explicit OAuthConnection(QObject *parent = 0);
    virtual ~OAuthConnection();

    virtual void connectToHost(const QString &login, const QString &password);
    virtual void disconnectFromHost();
    virtual QNetworkReply *request(const QString &method, const QVariantMap &args = QVariantMap());
    virtual Client::State connectionState() const;
    virtual int uid() const;
    virtual void clear();

    QByteArray accessToken() const;
    time_t expiresIn() const;
    void setAccessToken(const QByteArray &token, time_t expiresIn = 0);
    void setUid(int uid);
    int appId() const;
    void setAppId(int appId);
signals:
    void authConfirmRequested(QWebPage *page);
    void accessTokenChanged(const QByteArray &token, time_t expiresIn);
    void appIdChanged(int appId);
protected:
    QScopedPointer<OAuthConnectionPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_loadFinished(bool))
};

} // namespace Vreen

#endif // VK_OAUTHCONNECTION_H

