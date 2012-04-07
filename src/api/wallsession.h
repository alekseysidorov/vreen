#ifndef VK_WALLSESSION_H
#define VK_WALLSESSION_H

#include "client.h"
#include "wallpost.h"

namespace vk {

class Reply;
class WallSessionPrivate;
class VK_SHARED_EXPORT WallSession : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WallSession)
    Q_ENUMS(Filter)
public:
    enum Filter {
        Owner   = 0x1,
        Others   = 0x2,
        All     = Owner | Others
    };

    explicit WallSession(Contact *contact);
    Contact *contact() const;
    virtual ~WallSession();
    Reply *getPosts(Filter filter = All, quint8 count = 16, int offset = 0, bool extended = false);
signals:
    void postAdded(const vk::WallPost &post);
    void postDeleted(int id);
protected:
    QScopedPointer<WallSessionPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_posts_received(QVariant))
};

} // namespace vk

#endif // VK_WALLSESSION_H
