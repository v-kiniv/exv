#include "vitem.h"

VItem::VItem(const QString &name,
             const QString &desc,
             const int &filesCount,
             const QString &exid,
             const QString &image,
             QObject *parent) :
  ListItem(parent),
  m_name(name),
  m_desc(desc),
  m_exid(exid),
  m_image(image),
  m_filesCount(filesCount)
{
}

void VItem::setName(QString name)
{
  if(m_name != name) {
    m_name = name;
    emit dataChanged();
  }
}

QHash<int, QByteArray> VItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[NameRole] = "name";
  names[DescRole] = "desc";
  names[FilesCountRole] = "filesCount";
  names[ExidRole] = "exid";
  names[ImageRole] = "image";
  return names;
}

QVariant VItem::data(int role) const
{
  switch(role) {
  case NameRole:
    return name();
  case DescRole:
    return desc();
  case FilesCountRole:
    return filesCount();
  case ExidRole:
    return exid();
  case ImageRole:
    return image();
  default:
    return QVariant();
  }
}
