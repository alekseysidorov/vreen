#ifndef VREEN_LOCALSTORAGE_H
#define VREEN_LOCALSTORAGE_H
#include "contact.h"

namespace Vreen {

class AbstractLocalStorage
{
public:
    AbstractLocalStorage();
    virtual ~AbstractLocalStorage() {}
protected:
    virtual void loadBuddies(Roster *roster) = 0;
    virtual void storeBuddies(Roster *roster) = 0;
    //TODO group managers
    //key value storage
    virtual void store(const QString &key, const QVariant &value) = 0;
    virtual QVariant load(const QString &key) = 0;
    //TODO messages history async get and set
};

} // namespace Vreen

#endif // VREEN_LOCALSTORAGE_H
