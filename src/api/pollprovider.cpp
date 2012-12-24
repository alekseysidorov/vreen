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
** along with this program.  If not, see http://www.gnu.org/licees/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
#include "pollprovider.h"

namespace Vreen {

class PollProvider;
class PollProviderPrivate
{
    Q_DECLARE_PUBLIC(PollProvider)
public:
    PollProviderPrivate(PollProvider *q) : q_ptr(q) {}
    PollProvider *q_ptr;
};

PollProvider::PollProvider(Client *client) :
    QObject(client),
    d_ptr(new PollProviderPrivate(this))
{
}

PollProvider::~PollProvider()
{
}

} // namespace Vreen
