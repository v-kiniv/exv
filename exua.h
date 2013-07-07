#ifndef EXUA_H
#define EXUA_H

#include "listmodel.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Exua : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *m_nManager;
    QNetworkRequest m_nRequest;
    QNetworkReply *m_nReply;

    ListModel *m_searchModel;
public:
    explicit Exua(QObject *parent = 0);

    void searchVideo(QString str);

    ListModel* searchModel();
    
signals:

private slots:

public slots:
    void searchReply(QNetworkReply*);

};

#endif // EXUA_H
