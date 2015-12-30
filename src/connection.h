#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsLineItem>
#include "neededTypes.h"

class Connection : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 1001 };
    Connection( ElectronicsItem* item1,
                ElectronicsItem* item2,
                ConnectionType   connType,
                QGraphicsScene*  scene );
    ~Connection();

    int type() const;
    int connType() const;

    void updateLine();
    void connectLimbs();
    void disconnectLimbs();
    ElectronicsItem* limb1() const;
    ElectronicsItem* limb2() const;
    void setLimb1( ElectronicsItem* item );
    void setLimb2( ElectronicsItem* item );
    bool isLimb( ElectronicsItem* item );
    bool isLimbs( ElectronicsItem* item1, ElectronicsItem* item2 );

private:
    ConnectionType m_type;
    ElectronicsItem* m_limb1;
    ElectronicsItem* m_limb2;
};

#endif // CONNECTION_H
