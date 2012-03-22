#ifndef VK_REPLY_H
#define VK_REPLY_H

#include <QObject>
#include "vk_global.h"

class QNetworkReply;
namespace vk {

class ReplyPrivate;
class VK_SHARED_EXPORT Reply : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Reply)
public:

    enum ErrorCode {
        CaptchaNeeded = 14
    };

    explicit Reply(QNetworkReply *reply = 0);
    virtual ~Reply();
    QNetworkReply *reply() const;
    QVariant response() const;
    QVariant error() const;
signals:
    void resultReady(const QVariant &variables);
    void error(int code);
protected:
    void setReply(QNetworkReply *reply);
    QScopedPointer<ReplyPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_reply_finished())
    Q_PRIVATE_SLOT(d_func(), void _q_network_reply_destroyed(QObject*))
};

} // namespace vk

#endif // VK_REPLY_H
