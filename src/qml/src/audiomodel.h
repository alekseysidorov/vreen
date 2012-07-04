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
#ifndef AUDIOMODEL_H
#define AUDIOMODEL_H
#include <audio.h>
#include <QWeakPointer>
#include <client.h>

namespace vk {
class Contact;
}

class AudioModel : public vk::AudioModel
{
    Q_OBJECT

    Q_PROPERTY(QObject* client READ clientObj WRITE setClient NOTIFY clientChanged)
public:
    explicit AudioModel(QObject *parent = 0);
    vk::Client* client() const;
    void setClient(vk::Client* client);
public slots:
    void getContactAudio(vk::Contact *owner, int count = 100, int offset = 0);
    void searchAudio(const QString &query, int count = 50, int offset = 0);
signals:
    void clientChanged(QObject*);
    void requestFinished();
private:
    QObject* clientObj() const;
    void setClient(QObject* client);

    QWeakPointer<vk::Client> m_client;
    QWeakPointer<vk::AudioProvider> m_provider;
};

#endif // AUDIOMODEL_H

