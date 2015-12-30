#ifndef NEEDEDTYPES_H
#define NEEDEDTYPES_H

#include <QDebug>
#include <QtGlobal>

#include <QGraphicsPixmapItem>
#define SceneRect QRect( 0, 0, 2000, 1500 )

class ElectronicsItem;
class Connection;

enum ElectronicsType {
    NoElectronics   = QGraphicsPixmapItem::UserType + 100,
    PC              = QGraphicsPixmapItem::UserType + 101,
    Phone           = QGraphicsPixmapItem::UserType + 102,
    IPhone          = QGraphicsPixmapItem::UserType + 103,
    Router          = QGraphicsPixmapItem::UserType + 104,
    Server          = QGraphicsPixmapItem::UserType + 105,
    Station         = QGraphicsPixmapItem::UserType + 106,
    Satellite       = QGraphicsPixmapItem::UserType + 107
};

enum ConnectionType {
    NoConnection    = 1,
    Lan             = 2,
    Wan             = 3,
    WiFi            = 4
};

typedef QMap<ElectronicsItem*, ConnectionType> ElConnMap;
typedef QList<ElectronicsItem*> ElList;
typedef QList<Connection*> ConnList;
typedef QPointF Position;

// Auxiliary functions
QString typeToName( int type );
int nameToType( QString name );
QPixmap typeToPixmap( int type );



#endif // NEEDEDTYPES_H


