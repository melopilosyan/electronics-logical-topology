#ifndef ELSERVERCONNECTION_H
#define ELSERVERCONNECTION_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QHttp>
#include <QFile>
#include <QBuffer>
class ElServerConnection : public QHttp
{
    Q_OBJECT
public:
    explicit ElServerConnection( QObject* parent = 0 );
    ~ElServerConnection();

    QString getFile();

public slots:
    void onHttpRequestFinished( bool );
    void readyRead(QHttpResponseHeader resp);

private:
    QFile file;
    QByteArray bytes;
};

#endif // ELSERVERCONNECTION_H
