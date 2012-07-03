#ifndef DIALOGSMODEL_H
#define DIALOGSMODEL_H
#include <messagemodel.h>
#include <roster.h>
#include <QWeakPointer>

class DialogsModel : public vk::MessageListModel
{
    Q_OBJECT

    Q_PROPERTY(QObject* client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(int unreadCount READ unreadCount NOTIFY unreadCountChanged)
public:
    explicit DialogsModel(QObject *parent = 0);

    //HACK workaround about "Unable to assign QObject* to void"
    void setClient(QObject *client);
    //void setClient(vk::Client *client);
    //vk::Client *client() const;
    QObject *client() const;

    void setUnreadCount(int count);
    int unreadCount() const;
public slots:
    void getLastDialogs(int count = 16, int previewLength = -1);
signals:
    //void clientChanged(vk::Client*);
    void clientChanged(QObject*); //HACK
    void unreadCountChanged(int count);
    void requestFinished();
protected:
    virtual void doReplaceMessage(int index, const::vk::Message &message);
    virtual void doInsertMessage(int index, const::vk::Message &message);
    virtual void doRemoveMessage(int index);
private slots:
    void onDialogsReceived(const QVariant &dialogs);
	void onAddMessage(const vk::Message &message);
private:
    QWeakPointer<vk::Client> m_client;
    int m_unreadCount;
};

#endif // DIALOGSMODEL_H
