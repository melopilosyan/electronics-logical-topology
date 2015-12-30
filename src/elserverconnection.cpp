#include "elserverconnection.h"
#include "neededTypes.h"

#include <QDir>

ElServerConnection::ElServerConnection( QObject* parent ) :
    QHttp( parent ),
    file( "temp.xml" )
{
    setHost( "46.19.100.138", 8080 );
    connect( this, SIGNAL( readyRead(QHttpResponseHeader)), this, SLOT( readyRead(QHttpResponseHeader)));
    connect(this, SIGNAL(done(bool)),this, SLOT(onHttpRequestFinished(bool)));
    QDir tempDir("/tmp");
    if( !tempDir.exists("/tmp/GITC-Mobile_2-2011") )
        tempDir.mkdir("GITC-Mobile_2-2011");
    QDir::setCurrent("/tmp/GITC-Mobile_2-2011");
}

ElServerConnection::~ElServerConnection()
{}

QString ElServerConnection::getFile()
{
    get( "/webservice/" );
    return file.fileName();
}

//when processing the request identified by id has finished
void ElServerConnection::onHttpRequestFinished( bool error)
{   
    if (error) {
        qDebug()<< "saveRequested ERROR:" << errorString();
        return;
    }
    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
}

void ElServerConnection::readyRead( QHttpResponseHeader resp )
{
    Q_UNUSED( resp );
    bytes.append( readAll().trimmed() ) ;
}
