#include "pitem.h"

PItem::PItem(const QString &name,
             const QString &url,
             QObject *parent) :
  ListItem(parent),
  m_name(name),
  m_url(url)
{
}

void PItem::setName(QString name)
{
  if(m_name != name) {
    m_name = name;
    emit dataChanged();
  }
}

QHash<int, QByteArray> PItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[NameRole] = "name";
  names[UrlRole] = "url";
  return names;
}

QVariant PItem::data(int role) const
{
  switch(role) {
  case NameRole:
    return name();
  case UrlRole:
    return url();
  default:
    return QVariant();
  }
}
