#ifndef VK_REPLY_H
#define VK_REPLY_H

#include <QObject>

class QNetworkReply;
namespace vk {

class ReplyPrivate;
class Reply : public QObject
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
protected:
    void setReply(QNetworkReply *reply);
    QScopedPointer<ReplyPrivate> d_ptr;
};

} // namespace vk

#endif // VK_REPLY_H
