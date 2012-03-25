#include "newsmodel_p.h"

namespace vk {

NewsModel::NewsModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new NewsModelPrivate(this))
{

}

NewsModel::~NewsModel()
{

}


} //namespace vk
