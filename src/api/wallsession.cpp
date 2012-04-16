#include "wallsession.h"
#include "contact.h"
#include "wallpost.h"

namespace vk {

static const char *filters[] = {
    "owner",
    "others",
    "all"
};

class WallSession;
class WallSessionPrivate
{
    Q_DECLARE_PUBLIC(WallSession)
public:
    WallSessionPrivate(WallSession *q, Contact *contact) : q_ptr(q), contact(contact) {}
    WallSession *q_ptr;
    Contact *contact;

    void _q_posts_received(const QVariant &response)
    {
        auto list = response.toList();
        list.takeFirst();
        foreach (auto item, list) {
            auto post = WallPost::fromData(item, contact->client());
            emit q_func()->postAdded(post);
        }
    }
};

WallSession::WallSession(Contact *contact) :
    QObject(contact),
    d_ptr(new WallSessionPrivate(this, contact))
{

}

WallSession::~WallSession()
{

}

/*!
 * \brief WallSession::getPosts. A wrapper on API method wall.get, \link http://vk.com/developers.php?oid=-1&p=wall.get
 * \param filter determine what types of messages on the wall to get. The following parameter values​​:
 * Owner - messages on the wall by its owner
 * Others - posts on the wall, not on its owner
 * All - all the messages on the wall
 * \param count
 * \param offset
 * \param extended flag: true - three arrays will be returned to wall, profiles, and groups. By default, additional fields will not be returned.
 * \return
 */
Reply *WallSession::getPosts(WallSession::Filter filter, quint8 count, int offset, bool extended)
{
    Q_D(WallSession);
    QVariantMap args;
    args.insert("owner_id", QString::number(d->contact->id()));
    args.insert("offset", offset);
    args.insert("count", count);
    args.insert("filter", filters[filter-1]);
    args.insert("extended", extended);
    auto reply = d->contact->client()->request("wall.get", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_posts_received(QVariant)));

    return reply;
}

Contact *WallSession::contact() const
{
    return d_func()->contact;
}

} // namespace vk

#include "moc_wallsession.cpp"
