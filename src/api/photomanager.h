#ifndef VK_PHOTOMANAGER_H
#define VK_PHOTOMANAGER_H

#include <QObject>
#include "vk_global.h"

namespace vk {

class Client;
class PhotoManager : public QObject
{
    Q_OBJECT
public:
    explicit PhotoManager(Client *client);
    
public slots:
    
};

} // namespace vk

#endif // VK_PHOTOMANAGER_H
