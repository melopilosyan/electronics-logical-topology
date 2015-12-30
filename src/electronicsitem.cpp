#include "electronicsitem.h"

QVector<int> ElectronicsItem::nameCounter( 7, 0 );
ElectronicsItem::ElectronicsItem( ElectronicsType type,
                                  const Position& pos,
                                  QGraphicsScene* scene ):
    QGraphicsPixmapItem( 0, scene ),
    m_type( type ),
    m_name( defaultName() ),
    m_ipAddress( "" ),
    m_macAddress( mac() ),
    m_connects()
{
    setFlags( QGraphicsItem::ItemIsMovable    |
              QGraphicsItem::ItemIsFocusable  |
              QGraphicsItem::ItemIsSelectable );
    setPixmap( typeToPixmap( type ) );
    setCenter( pos );
    setToolTip();
}

ElectronicsItem::~ElectronicsItem()
{
}

int ElectronicsItem::type() const
{
    return Type;
}
int ElectronicsItem::elType() const
{   return m_type;   }

QString ElectronicsItem::name() const
{   return m_name;   }

QString ElectronicsItem::ipAddress() const
{   return m_ipAddress;   }

QString ElectronicsItem::macAddress() const
{   return m_macAddress;   }

Position ElectronicsItem::center() const
{
    Position center( ( m_type == Router ) ? boundingRect().center() + Position( 0, 15 ):
                                            boundingRect().center() );
    return mapToScene( center );
}

bool ElectronicsItem::isConnected( ElectronicsItem* item ) const
{
    return ( m_connects.end() == m_connects.find( item ) ) ? false: true;
}

ElConnMap ElectronicsItem::connects() const
{
    return m_connects;
}

bool ElectronicsItem::connect( ConnectionType connType, ElectronicsItem* item )
{
    if( m_connects.find( item ) == m_connects.end() ) {
        m_connects.insert( item, connType );
        return true;
    }
    return false;
}

void ElectronicsItem::disconnect()
{
    if( m_connects.isEmpty() ) return;
    ElConnMap::iterator it = m_connects.begin();
    for( ; it != m_connects.end(); it++ ) {
            it.key()->disconnect( this );
    }
    m_connects.clear();
}

void ElectronicsItem::disconnect( ElectronicsItem* item )
{
    m_connects.remove( item );
    if( item->isConnected( this ) )
        item->disconnect( this );
}

void ElectronicsItem::setName( const QString& name )
{
    m_name = name;
    setToolTip();
}

void ElectronicsItem::setIpAddress( const QString& ip )
{
    m_ipAddress = ip;
    setToolTip();
}

void ElectronicsItem::setMacAddress( const QString& mac )
{
    m_macAddress = mac;
    setToolTip();
}

void ElectronicsItem::init( const QString& name,
                            const QString& ip,
                            const QString& mac )
{
    m_name = name;
    m_ipAddress = ip;
    m_macAddress = mac;
    setToolTip();
}

void ElectronicsItem::init( const QStringList& sl )
{
    m_name = sl.value( 0 );
    m_ipAddress = sl.value( 1 );
    m_macAddress = sl.value( 2 );
    setToolTip();
}

void ElectronicsItem::setCenter( const Position& pos )
{
    QSizeF size = boundingRect().size();
    setPos( pos + Position( -( size.width() / 2 ), -( size.height() / 2 ) ) );
}

void ElectronicsItem::setToolTip()
{
    QString toolTip = QObject::tr( "Name:\t%1\nIP:\t%2\nMAC:\t%3" )
            .arg( m_name ).arg( m_ipAddress ).arg( m_macAddress );
    QGraphicsPixmapItem::setToolTip( toolTip );
}

QString ElectronicsItem::mac() const
{
    static QVector<char> letters( QVector<char>() << 'A' << 'B' << 'C' << 'D' << 'E' << 'F' );
    if( m_type == NoElectronics ) return "";
    int rand = qrand() % 100;
    QString mac;
    for( int i = 0; i < 12; i++ ) {
            rand += i + rand/2;
        switch( ( rand + i )% 10 ) {
            case 0: mac += letters[ rand % 6 ]; break;
            case 1: mac += letters[ rand % 6 ]; break;
            case 2: mac += QString::number( rand % 10 ); break;
            case 3: mac += letters[ rand % 6 ]; break;
            case 4: mac += QString::number( rand % 10 ); break;
            case 5: mac += letters[ rand % 6 ]; break;
            case 6: mac += QString::number( rand % 10 ); break;
            case 7: mac += letters[ rand % 6 ]; break;
            case 8: mac += QString::number( rand % 10 ); break;
            case 9: mac += letters[ rand % 6 ]; break;
        default: break;
        }
        if( i != 11 && i % 2 )
            mac += ":";
    }
    return mac;
}

QString ElectronicsItem::defaultName() const
{
    if( m_type == NoElectronics ) return "";
    int typeNum = 0;
    QString typeName = "";
    switch( m_type ) {
        case PC: typeNum = 0; typeName = "PC "; break;
        case IPhone: typeNum = 1; typeName = "IPhone "; break;
        case Phone: typeNum = 2; typeName = "Phone "; break;
        case Router: typeNum = 3; typeName = "Router "; break;
        case Server: typeNum = 4; typeName = "Server "; break;
        case Satellite: typeNum = 5; typeName = "Satellite "; break;
        case Station: typeNum = 6; typeName = "Station "; break;
    default: break;
    }
    typeName += QString::number( ++nameCounter[typeNum] );
    return typeName;
}
