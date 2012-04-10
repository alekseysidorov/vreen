#ifndef VK_AUDIOITEM_H
#define VK_AUDIOITEM_H

#include <QSharedDataPointer>
#include "vk_global.h"

class QUrl;

namespace vk {

class Client;
class AudioItemData;

class VK_SHARED_EXPORT AudioItem
{
public:
    AudioItem(Client *client);
    AudioItem(const AudioItem &);
    AudioItem &operator=(const AudioItem &);
    ~AudioItem();

    bool operator <(const AudioItem &other) const;
    
    int id() const;
    void setId(int aid);
    int ownerId() const;
    void setOwnerId(int ownerId);
    QString artist() const;
    void setArtist(const QString &artist);
    QString title() const;
    void setTitle(const QString &title);
    qreal duration() const;
    void setDuration(qreal duration);
    QUrl url() const;
    void setUrl(const QUrl &url);
    int lyricsId() const;
    void setLyricsId(int lyricsId);
    int albumId() const;
    void setAlbumId(int albumId);
private:
    QSharedDataPointer<AudioItemData> data;
};
typedef QList<AudioItem> AudioItemList;

} // namespace vk

#endif // VK_AUDIOITEM_H
