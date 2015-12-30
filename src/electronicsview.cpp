#include "electronicsview.h"
#include "electronicsitem.h"
#include "elinitdialog.h"
#include "neededTypes.h"
#include "connection.h"
#include "mainwindow.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDropEvent>


ElectronicsView::ElectronicsView( MainWindow* core, QWidget* parent ):
    QGraphicsView( new QGraphicsScene( SceneRect, parent ), parent),
    conn( 0 ),
    m_core( core ),
    elMouse( 0 ),
    newConnection( false )
{
    setAcceptDrops( true );
    setTransformationAnchor( QGraphicsView::NoAnchor );
    scene()->setBackgroundBrush( QColor( 100, 100, 100, 150 ) );
}

ElectronicsView::~ElectronicsView()
{
    delete scene();
}

void ElectronicsView::mousePressEvent( QMouseEvent* event )
{
    if( event->button() == Qt::LeftButton ) {
        ElectronicsType eltype = m_core->selectedElectronics();
        ConnectionType conntype = m_core->selectedConnection();
        Position mousePos = mapToScene( event->pos() );
        if( eltype != NoElectronics ) {
            ElectronicsItem* el = new ElectronicsItem( eltype, mousePos, scene() );
            m_core->addElectronics( el );
        } else if( newConnection ) {
            QGraphicsItem* gItem = scene()->itemAt( mousePos );
            if( !gItem ) return;
            if( gItem->type() == ElectronicsItem::Type ) {
                ElectronicsItem* item = (ElectronicsItem*)gItem;
                if( !item->isConnected( conn->limb2() ) ) {
                    Connection* newConn = new Connection( item, conn->limb2(),
                                       (ConnectionType)conn->connType(), scene() );
                    newConn->connectLimbs();
                    m_core->addConnection( newConn );
                    scene()->update();
                }
            }
            removeHalfLine();
        } else if( conntype != NoConnection && !newConnection ) {
            QGraphicsItem* gItem = scene()->itemAt( mousePos );
            if( !gItem ) return;
            if( gItem->type() != ElectronicsItem::Type ) return;
            ElectronicsItem* item = (ElectronicsItem*)gItem;
            elMouse = new ElectronicsItem( NoElectronics, mousePos );
            conn = new Connection( elMouse, item, conntype, scene() );
            setMouseTracking( true );
            newConnection = true;
        }
    }
    QGraphicsView::mousePressEvent( event );
}

void ElectronicsView::mouseMoveEvent( QMouseEvent* event )
{
    if( newConnection ) {
        elMouse->setCenter( mapToScene( event->pos() ) );
        conn->updateLine();
    }
    if( scene()->items().isEmpty() ) return;
    m_core->updateConnections();
    scene()->update();
    QGraphicsView::mouseMoveEvent( event );
}

void ElectronicsView::mouseDoubleClickEvent( QMouseEvent *event )
{
    QGraphicsItem* item = scene()->itemAt( mapToScene( event->pos() ) );
    if( !item ) return;
    if( item->type() == ElectronicsItem::Type ) {
        ElInitDialog* dialog = new ElInitDialog( ( ElectronicsItem* )item, this);
        dialog->show();
    }
}

void ElectronicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasFormat( "application/electronicsItem" ) ) {
        //event->setAccepted( true );
        event->acceptProposedAction();
        qDebug() << "ElectronicsView::dragEnterEvent in\naccepted";
    } else {
        event->setAccepted( false );
    }
}

void ElectronicsView::dropEvent(QDropEvent *event)
{
        qDebug() << "ElectronicsView::dropEvent";
        const QMimeData* mimeData = event->mimeData();
        ElectronicsItem* elItem = new ElectronicsItem(
                    (ElectronicsType)nameToType( mimeData->text() ),
                    Position( event->pos() ), scene() );
        m_core->addElectronics( elItem );
        qDebug() << "Dropped";
}

bool ElectronicsView::hasHalfLine() const
{    return newConnection;   }

bool ElectronicsView::hasSelectedItems() const
{
    return  !scene()->selectedItems().isEmpty();
}

void ElectronicsView::removeHalfLine()
{
    if( conn ) delete conn;
    if( elMouse ) delete elMouse;
    conn = 0;
    elMouse = 0;
    newConnection = false;
    setMouseTracking( false );
}

