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
#include "audiomodel.h"
#include <contact.h>
#include <reply.h>

AudioModel::AudioModel(QObject *parent) :
    Vreen::AudioModel(parent)
{
}

Vreen::Client *AudioModel::client() const
{
    return m_client.data();
}

void AudioModel::setClient(Vreen::Client *client)
{
    if (m_client.data() == client)
        return;
    if (!client) {
        m_provider.data()->deleteLater();
        emit clientChanged(client);
        return;
    }

    auto provider = new Vreen::AudioProvider(client);
    connect(provider, SIGNAL(audioItemReceived(Vreen::AudioItem)), SLOT(addAudio(Vreen::AudioItem)));
    m_provider = provider;
    m_client = client;
    emit clientChanged(client);
}

void AudioModel::getContactAudio(Vreen::Contact *owner, int count, int offset)
{
    if (m_provider.data()) {
        auto reply = m_provider.data()->getContactAudio(owner->id(), count, offset);
        connect(reply, SIGNAL(resultReady(QVariant)),
                this, SIGNAL(requestFinished()));
    }
}

void AudioModel::searchAudio(const QString &query, int count, int offset)
{
    if (m_provider.data()) {
        auto reply = m_provider.data()->searchAudio(query, count, offset);
        connect(reply, SIGNAL(resultReady(QVariant)),
                this, SIGNAL(requestFinished()));
    }
}

QObject *AudioModel::clientObj() const
{
    return client();
}

void AudioModel::setClient(QObject *client)
{
    setClient(static_cast<Vreen::Client*>(client));
}

