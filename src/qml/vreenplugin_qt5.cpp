#include "vkitqmlplugin.h"

class VKitQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri)
    {
        registerVreenTypes(uri);
    }
};

#include "vreenplugin_qt5.moc"
