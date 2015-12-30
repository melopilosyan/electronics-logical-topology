#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "neededTypes.h"
#include <QDomElement>


class ElectronicsXmlParser
{

public:
    ElectronicsXmlParser();
    ~ElectronicsXmlParser();

    static bool read( const QString& fn, ElList& elList );
    static void write( const QString& fn, const ElList& elList );

private:
    // WRITE
    static QDomElement makeElement( QDomDocument&    domDoc,
                                    const QString&   name,
                                    const QString&   attrType = QString::null,
                                    const QString&   attrValue = QString::null,
                                    const QString&   text = QString::null );
    static QDomElement electronicsNod( QDomDocument&    domDoc,
                                       ElectronicsItem* item );
    static QDomElement makeConnectsNod( QDomDocument& domDoc,
                                        const ElConnMap& elConnMap );
    static QDomElement makePosNod( QDomDocument& domDoc, const Position& pos );
    // READ
    static void createElList( ElList& elList, QDomDocument& domDoc );
    static ElectronicsItem* createElectronics( QDomElement& domElement );
    static Position createPos( QDomElement& domElement );
    static void createConnectionMap( QMap<int, QString>& connMap, QDomElement& domElement );
    static void createRealConnections( ElList& elList );

    static QMap<ElectronicsItem*, QMap<int, QString> > connections;
};

#endif // XMLPARSER_H
