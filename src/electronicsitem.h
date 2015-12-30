#ifndef ELECTRONICSITEM_H
#define ELECTRONICSITEM_H

#include "neededTypes.h"

class ElectronicsItem : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + 1000 };
    ElectronicsItem( ElectronicsType elType  ,
                     const Position& pos   = Position(),
                     QGraphicsScene* scene = 0 );
    ~ElectronicsItem();

    int type() const;
    int elType() const;                                             // return its type
    QString name() const;                                           // get name
    Position center() const;                                        // return bounding rect's--
                                                                    // --center maped to scene
    QString ipAddress() const;                                      // get IP address
    QString macAddress() const;                                     // get MAK address
    ElConnMap connects() const;                                     // get list of connected electronics
    bool isConnected( ElectronicsItem* item) const;                 // return true if connected--
                                                                    // --with item, otherwise false
    void setName( const QString& name );                            // set its name to name
    void setCenter( const Position& pos );                          // set its center to pos
    void setIpAddress( const QString& ip );                         // set IP address to ip
    void setMacAddress( const QString& mac );                       // set MAK address to mak
    void disconnect();                                              // disconnect all connections
    void disconnect( ElectronicsItem* item );                       // return true if disconnect with item
    bool connect( ConnectionType connType, ElectronicsItem* item ); // return true if connection with item--
                                                                    // --established, otherwise false
    void init( const QStringList& sl );
    void init( const QString& name, const QString& ip, const QString& mac );


private:
    static QVector<int> nameCounter;
    ElectronicsType m_type;
    QString m_name;
    QString m_ipAddress;
    QString m_macAddress;
    ElConnMap m_connects;

    void setToolTip();
    QString mac() const;
    QString defaultName() const;
};

#endif // ELECTRONICSITEM_H
