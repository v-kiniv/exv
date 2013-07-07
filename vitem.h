#include "listmodel.h"

class VItem : public ListItem
{
  Q_OBJECT

public:
  enum Roles {
    NameRole = Qt::UserRole+1,
    DescRole,
    FilesCountRole,
    ExidRole,
    ImageRole
  };

public:
  VItem(QObject *parent = 0): ListItem(parent) {}
  explicit VItem(const QString &name, const QString &desc, const int &filesCount, const QString &exid, const QString &image, QObject *parent = 0);
  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;
  void setName(QString name);
  inline QString id() const { return m_name; }
  inline QString name() const { return m_name; }
  inline QString desc() const { return m_desc; }
  inline int filesCount() const { return m_filesCount; }
  inline QString exid() const { return m_exid; }
  inline QString image() const { return m_image; }

private:
  QString m_name;
  QString m_desc;
  int m_filesCount;
  QString m_exid;
  QString m_image;
};
