#include "electronicsitemtoolbutton.h"
#include "electronicsitem.h"
#include "neededTypes.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMimeData>
#include <QBuffer>
#include <QAction>
#include <QDebug>
#include <QDrag>


ElectronicsToolButton::ElectronicsToolButton() {}

ElectronicsToolButton::ElectronicsToolButton(QWidget *parent)
        : QToolButton( parent )
{
    setAutoRaise( true );
}

void ElectronicsToolButton::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton ) {
            m_ptDragPos = event->pos();
    }
}

void ElectronicsToolButton::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "ElectronicsToolButton::mouseMoveEvent";
    if( event->buttons() & Qt::LeftButton ) {
        int distance = ( event->pos() - m_ptDragPos ).manhattanLength();
        if( distance < QApplication::startDragDistance() )
                return;
    }
    QAction* action = defaultAction();
    QDrag* drag = new QDrag( this );
    QMimeData *mimeData = new QMimeData;
    mimeData->setData( "application/electronicsItem", QByteArray() );
    mimeData->setText( action->text() );
    QPixmap pixmap = typeToPixmap( nameToType( action->text() ) );
    drag->setHotSpot( QPoint( pixmap.width()/2-10, pixmap.height()/2-10 ) );
    drag->setMimeData( mimeData );
    drag->setPixmap( pixmap );
    qDebug() << "Drag created";
    drag->exec();
}
