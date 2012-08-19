#include "vkitqmlplugin.h"

class VKitQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        registerVreenTypes(uri);
    }
};

Q_EXPORT_PLUGIN2(vkitplugin, VKitQmlPlugin)

#include "vreenplugin_qt4.moc"
