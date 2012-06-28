#ifndef VK_CONTENTDOWNLOADER_H
#define VK_CONTENTDOWNLOADER_H

#include <QObject>
#include "vk_global.h"

class QUrl;

namespace vk {

class VK_SHARED_EXPORT ContentDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ContentDownloader(QObject *parent = 0);
    Q_INVOKABLE QString download(const QUrl &link);
signals:
    void downloadFinished(const QString &fileName);
private slots:
    void replyDone();
};

} // namespace vk

#endif // VK_CONTENTDOWNLOADER_H
