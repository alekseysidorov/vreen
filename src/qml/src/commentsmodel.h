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
#ifndef COMMENTSMODEL_H
#define COMMENTSMODEL_H

#include <QAbstractListModel>
#include <QPointer>
#include <commentssession.h>

class CommentsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Vreen::Contact* contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(int postId READ postId WRITE setPostId NOTIFY postChangedId)
public:
    explicit CommentsModel(QObject *parent = 0);

    enum Roles {
        IdRole = Qt::UserRole,
        FromRole,
        DateRole,
        BodyRole,
        LikesRole
    };

    Vreen::Contact* contact() const;
    void setContact(Vreen::Contact* contact);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &) const;
    int count() const;
    int findComment(int id) const;
    int postId() const;
    void setPostId(int arg);
signals:
    void contactChanged(Vreen::Contact*);
    void requestFinished();
    void postChangedId(int);

public slots:
    void clear();
    void getComments(int offset = 0, int count = 100);
private slots:
    void addComment(const QVariantMap &data);
    void deleteComment(int id);
private:
    QPointer<Vreen::Contact> m_contact;
    QPointer<Vreen::CommentSession> m_session;
    Vreen::CommentList m_comments;
    int m_postId;
};

#endif // COMMENTSMODEL_H

