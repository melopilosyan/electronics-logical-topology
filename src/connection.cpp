#include "electronicsitem.h"
#include "connection.h"

#include <QPen>


QPen typeToPen( ConnectionType type )
{
    switch( type ) {
        case Lan: return QPen( QBrush( Qt::red ), 3.8 );
        case WiFi: return QPen( QBrush( Qt::blue ), 3.8, Qt::DashLine );
        case Wan: return QPen( QBrush( Qt::red ), 3.8 );
    default: break;
    }
    return QPen();
}

Connection::Connection( ElectronicsItem* item1,
                        ElectronicsItem* item2,
                        ConnectionType   type,
                        QGraphicsScene*  scene ):
    QGraphicsLineItem( 0, scene ),
    m_type( type ),
    m_limb1( item1 ),
    m_limb2( item2 )
{
    updateLine();
    setZValue( -1 );
    setPen( typeToPen( type ) );
    setFlags( QGraphicsItem::ItemIsSelectable );
    setToolTip( QObject::tr( "%1 <-> %2" ).arg( item1->name() ).arg( item2->name() ) );
}

Connection::~Connection()
{   }

int Connection::type() const
{   return Type;   }

int Connection::connType() const
{   return m_type;   }

void Connection::updateLine()
{
    QLineF line( m_limb1->center(), m_limb2->center() );
    setLine( line );
}

void Connection::connectLimbs()
{
    m_limb1->connect( m_type, m_limb2 );
    m_limb2->connect( m_type, m_limb1 );
}

void Connection::disconnectLimbs()
{
    m_limb1->disconnect( m_limb2 );
    m_limb2->disconnect( m_limb1 );
}

void Connection::setLimb1( ElectronicsItem* item )
{
    disconnectLimbs();
    m_limb1 = item;
    connectLimbs();
    updateLine();
}

void Connection::setLimb2( ElectronicsItem* item )
{
    disconnectLimbs();
    m_limb2 = item;
    connectLimbs();
    updateLine();
}

ElectronicsItem* Connection::limb1() const
{   return m_limb1;  }

ElectronicsItem* Connection::limb2() const
{   return m_limb2;  }

bool Connection::isLimb( ElectronicsItem* item )
{
    return ( m_limb1 == item || m_limb2 == item ) ? true: false;
}

bool Connection::isLimbs( ElectronicsItem* item1, ElectronicsItem* item2 )
{
    return ( ( m_limb1 == item1 && m_limb2 == item2 ) ||
             ( m_limb1 == item2 && m_limb2 == item1 ) ) ? true: false;
}
