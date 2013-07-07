#include "listmodel.h"

class PItem : public ListItem
{
  Q_OBJECT

public:
  enum Roles {
    NameRole = Qt::UserRole+1,
    UrlRole
  };

public:
  PItem(QObject *parent = 0): ListItem(parent) {}
  explicit PItem(const QString &name, const QString &url, QObject *parent = 0);
  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;
  void setName(QString name);
  inline QString id() const { return m_name; }
  inline QString name() const { return m_name; }
  inline QString url() const { return m_url; }

private:
  QString m_name;
  QString m_url;
};
