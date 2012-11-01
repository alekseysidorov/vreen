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
#ifndef WALLMODEL_H
#define WALLMODEL_H

#include <QAbstractListModel>
#include <contact.h>
#include <wallsession.h>
#include <QPointer>

class WallModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Vreen::Contact* contact READ contact WRITE setContact NOTIFY contactChanged)
public:
    enum Roles {
        IdRole = Qt::UserRole,
        FromRole,
        ToRole,
        DateRole,
        BodyRole,
        CommentsRole,
        LikesRole,
        RepostsRole,
        AttachmentsRole
    };

    explicit WallModel(QObject *parent = 0);
    Vreen::Contact* contact() const;
    void setContact(Vreen::Contact* arg);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &) const;
    int count() const;
public slots:
    void getLastPosts(int count = 25, Vreen::WallSession::Filter filter = Vreen::WallSession::All);
    void addLike(int postId, bool retweet = false, const QString &message = QString());
    void deleteLike(int postId);
    void clear();
    int findPost(int id);
signals:
    void contactChanged(Vreen::Contact* arg);
    void requestFinished();
private slots:
    void addPost(const Vreen::WallPost &post);
    void replacePost(int index, const Vreen::WallPost &post);
    void onPostLikeAdded(int id, int likes, int reposts, bool isRetweeted);
    void onPostLikeDeleted(int postId, int count);
private:
    Vreen::Roster *roster() const;
    QPointer<Vreen::Contact> m_contact;
    QPointer<Vreen::WallSession> m_session;
    Vreen::WallPostList m_posts;
};

#endif // WALLMODEL_H

