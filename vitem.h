#include "listmodel.h"
#include <QDataStream>

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
  explicit VItem(const QString &name, const QString &desc, const QString &filesCount, const QString &exid, const QString &image, QObject *parent = 0);
  QVariant data(int role) const;
  QHash<int, QByteArray> roleNames() const;
  void setName(QString name);
  void setCutCast(bool s) { m_cutCast = s; }
  void setCastBegin(qint64 pos) { m_castBegin = pos; }
  void setCastEnd(qint64 pos) { m_castEnd = pos; }
  inline QString id() const { return m_exid; }
  Q_INVOKABLE inline QString name() const { return m_name; }
  Q_INVOKABLE inline QString desc() const { return m_desc; }
  Q_INVOKABLE inline QString filesCount() const { return m_filesCount; }
  Q_INVOKABLE inline QString exid() const { return m_exid; }
  Q_INVOKABLE inline QString image() const { return m_image; }
  void write(QDataStream &stream);
  void read(QDataStream &stream);
  VItem *copy();

private:
  QString m_name;
  QString m_desc;
  QString m_filesCount;
  QString m_exid;
  QString m_image;
  bool m_cutCast;
  qint64 m_castBegin;
  qint64 m_castEnd;
};
