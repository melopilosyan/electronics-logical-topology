#include "mainwindow.h"

#include <QApplication>
#include <QTime>


int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    app.setOrganizationName( "GITC-Mobile_2-2011" );
    app.setApplicationName( "ElLogicalTop");
    qsrand( QTime::currentTime().second() );
    MainWindow window;
    window.show();
    return app.exec();
}
