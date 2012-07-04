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
#ifndef VK_AUDIO_H
#define VK_AUDIO_H

#include <QAbstractListModel>
#include "audioitem.h"
#include "abstractlistmodel.h"

namespace vk {

class Client;
class Reply;

class AudioProviderPrivate;
class VK_SHARED_EXPORT AudioProvider : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioProvider)
public:
    AudioProvider(Client *client);
    virtual ~AudioProvider();
    Reply *getContactAudio(int uid, int count = 50, int offset = 0);
    Reply *searchAudio(const QString &query, int count = 50, int offset = 0);
signals:
    void audioItemReceived(const vk::AudioItem &items);
protected:
    QScopedPointer<AudioProviderPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_audio_received(QVariant))
};

class AudioModelPrivate;
class VK_SHARED_EXPORT AudioModel : public AbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioModel)

    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
public:

    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        UrlRole,
        DurationRole,
        AlbumIdRole,
        LyricsIdRole,
        OwnerIdRole
    };

    AudioModel(QObject *parent);
    virtual ~AudioModel();

    int count() const;
    int findAudio(int id) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
    void setSortOrder(Qt::SortOrder order);
    Qt::SortOrder sortOrder() const;
public slots:
    void clear();
    void addAudio(const vk::AudioItem &item);
    void removeAudio(int aid);
signals:
    void sortOrderChanged(Qt::SortOrder);
protected:
    void insertAudio(int index, const AudioItem &item);
    void replaceAudio(int index, const AudioItem &item);
    virtual void sort(int column, Qt::SortOrder order);
private:
    QScopedPointer<AudioModelPrivate> d_ptr;
};

} // namespace vk

#endif // VK_AUDIO_H

