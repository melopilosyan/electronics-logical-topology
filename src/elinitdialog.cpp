#include "electronicsitem.h"
#include "elinitdialog.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFont>


ElInitDialog::ElInitDialog( ElectronicsItem* el, QWidget* parent ):
    QDialog( parent ),
    electronics( el ),
    nameEdit( new QLineEdit( electronics->name(), this ) ),
    ipEdit( new QLineEdit( electronics->ipAddress(), this ) ),
    macEdit( new QLineEdit( electronics->macAddress(), this ) )
{
    setFixedSize( 230, 200 );
    setWindowModality( Qt::WindowModal );
    setWindowTitle( "Electronics data" );

    createLabels();
    ipEdit->setGeometry( 80, 70, 140, 28 );
    ipEdit->setInputMask( "000.000.000.000" );
    nameEdit->setGeometry( 80, 25, 140, 28 );
    nameEdit->setSelection( 0, nameEdit->text().size() );
    macEdit->setGeometry( 80, 115, 140, 28 );
    macEdit->setInputMask( "HH:HH:HH:HH:HH:HH" );
    QPushButton* ok = new QPushButton( "OK", this );
    ok->setGeometry( width()-75, height()-40, 65, 28 );
    connect( ok, SIGNAL( clicked() ), this, SLOT( collectData() ) );
    QPushButton* cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( width()-150, height()-40, 65, 28 );
    connect( cancel, SIGNAL( clicked() ), this, SLOT( close() ) );
}

ElInitDialog::~ElInitDialog()
{
}

void ElInitDialog::collectData()
{
    electronics->init( nameEdit->text(), ipEdit->text(), macEdit->text() );
    close();
}

void ElInitDialog::createLabels()
{
    int y = 25;
    QLabel* label;
    QStringList strList( QStringList() << "Name" << "IP" << "MAC" );
    foreach( QString str, strList ) {
        label = new QLabel( str + " :", this );
        label->setFont( QFont( "Serif", 12, 20 ) );
        label->setGeometry( 15, y, 75, 28 );
        y += 45;
    }
}

