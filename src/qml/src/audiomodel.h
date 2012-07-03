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
