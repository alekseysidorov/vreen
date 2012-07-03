#ifndef CHATMODEL_H
#define CHATMODEL_H
#include "chatmodel.h"
#include <messagemodel.h>
#include <QWeakPointer>

namespace vk {
class ChatSession;
} //namespace vk

class ChatModel : public vk::MessageListModel
{
    Q_OBJECT
    Q_PROPERTY(vk::Contact* contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
public:
    explicit ChatModel(QObject *parent = 0);
    void setContact(vk::Contact *contact);
    vk::Contact *contact() const;
    QString title() const;
public slots:
    void getHistory(int count = 16, int offset = 0);
    void markAsRead(int mid, bool set = true);
signals:
    void contactChanged(vk::Contact*);
    void titleChanged(const QString &title);
    void requestFinished();
private slots:
    void messageReadStateChanged(int id, bool set);
private:
    QWeakPointer<vk::ChatSession> m_session;
};

#endif // CHATMODEL_H
