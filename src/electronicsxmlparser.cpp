#include "electronicsxmlparser.h"
#include "electronicsitem.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QDomAttr>
#include <QDomNode>
#include <QFile>
#include <QMap>


QMap<ElectronicsItem*, QMap<int, QString> > ElectronicsXmlParser::connections;
ElectronicsXmlParser::ElectronicsXmlParser() {}
ElectronicsXmlParser::~ElectronicsXmlParser() {}

void read() {}

bool ElectronicsXmlParser::read( const QString& fn, ElList& elList )
{
    QFile file( fn );
    if( file.open( QIODevice::ReadOnly ) ) {
        QDomDocument domDoc;
        QString errorMsg( "" );
        int errorLine( 0 ), errorColumn( 0 );
        bool contentSetted = domDoc.setContent( &file, &errorMsg, &errorLine, &errorColumn );
        file.close();
        if( contentSetted ) {
            createElList( elList, domDoc );
            createRealConnections( elList );
            connections.clear();
            return true;
        } else {
            QMessageBox::warning( 0, "Couldn't parse xml",
                                  QObject::tr("Error: %1\nLine %2 column %3.")
                                  .arg( errorMsg )
                                  .arg( QString::number( errorLine ) )
                                  .arg( QString::number( errorColumn ) ) );
            return false;
        }
    }
    return false;
}

void ElectronicsXmlParser::createElList( ElList& elList, QDomDocument& domDoc )
{
    QDomNode domNode = domDoc.firstChild().firstChild();
    while(!domNode.isNull()) {
        if (domNode.isElement()) {
         QDomElement domEl = domNode.toElement();
         elList << createElectronics( domEl );
        }
        domNode = domNode.nextSibling();
    }
}

ElectronicsItem* ElectronicsXmlParser::createElectronics( QDomElement& domElement )
{
    ElectronicsType type = (ElectronicsType)domElement.attributeNode("type").value().toInt();
    QDomNode domNode = domElement.firstChild();
    QMap<int, QString> connMap;
    QStringList elData;
    Position pos;

    while( !domNode.isNull() ) {
        if( domNode.isElement() ) {
            QDomElement domEl = domNode.toElement();
            if( domEl.tagName() == "Name" || domEl.tagName() == "IP" ||
                domEl.tagName() == "MAC" ) {
                elData << domEl.text();
            } else if( domEl.tagName() == "position" ) {
                pos = createPos( domEl );
            } else if( domEl.tagName() == "connects" ) {
                createConnectionMap( connMap, domEl );
            }
        }
        domNode = domNode.nextSibling();
    }
    ElectronicsItem* item = new ElectronicsItem( type, pos );
    item->init( elData );
    connections.insert( item, connMap );
    return item;
}

Position ElectronicsXmlParser::createPos( QDomElement& domElement )
{
    Position pos;
    QDomNode domNode = domElement.firstChild();
    pos.setX( domNode.toElement().text().toFloat() );
    pos.setY( domNode.nextSiblingElement("y").text().toFloat() );
    return pos;
}

void ElectronicsXmlParser::createConnectionMap( QMap<int, QString>& connMap,
                                                QDomElement&        domElement )
{
    QDomNode domNode = domElement.firstChild();
    while( !domNode.isNull() ) {
        QDomNode domChildNode = domNode.firstChild();
        int connType = domChildNode.toElement().text().toInt();
        QString name = domChildNode.nextSiblingElement("connectWith").text();
        connMap[connType] = name;
        domNode = domNode.nextSibling();
    }
}
ElectronicsItem* ptrOfItemInList( const QString& itemName, ElList& elList )
{
    foreach( ElectronicsItem* item, elList ) {
        if( item->name() == itemName ) {
            return item;
        }
    }
    return 0;
}
//
void ElectronicsXmlParser::createRealConnections( ElList& elList )
{
    QMap<ElectronicsItem*, QMap<int, QString> >::iterator iter = connections.begin();
    for( ; iter != connections.end(); iter++ ) {
        QMap<int, QString>::iterator it = iter.value().begin();
        for( ; it != iter.value().end(); it++ ) {
            ElectronicsItem* item = ptrOfItemInList( it.value(), elList );
            if( !item ) return;
            iter.key()->connect( (ConnectionType)it.key(), item );
            item->connect( (ConnectionType)it.key(), iter.key() );
        }
    }
}
void write(){}
//****************************************************************************
void ElectronicsXmlParser::write( const QString& fn, const ElList& elList )
{
    QDomDocument doc("ElectronicsLogicalTopology");
    QDomElement  domElement = doc.createElement("ELLogicalTop");
    doc.appendChild(domElement);
    foreach( ElectronicsItem* item, elList ) {
        domElement.appendChild(electronicsNod( doc, item ) );
    }
    QFile file( fn );
    if( file.open( QIODevice::WriteOnly ) ) {
        QTextStream(&file) << doc.toString();
        file.close();
    }
}

QDomElement ElectronicsXmlParser::makeElement( QDomDocument&  domDoc,
                                               const QString& name,
                                               const QString& attrType,
                                               const QString& attrValue,
                                               const QString& text )
{
    QDomElement domElement = domDoc.createElement( name );

    if (!attrValue.isEmpty()) {
        QDomAttr domAttr = domDoc.createAttribute( attrType );
        domAttr.setValue(attrValue);
        domElement.setAttributeNode(domAttr);
    }

    if (!text.isEmpty()) {
        QDomText domText = domDoc.createTextNode(text);
        domElement.appendChild(domText);
    }
    return domElement;
}

QDomElement ElectronicsXmlParser::electronicsNod( QDomDocument&    domDoc,
                                                  ElectronicsItem* item )
{
    QDomElement domElement = makeElement( domDoc, "electronics", "type",
                                          QString().setNum( item->elType() ) );
    domElement.appendChild(makeElement( domDoc, "Name", "", "", item->name() ) );
    domElement.appendChild(makeElement( domDoc, "IP", "", "", item->ipAddress() ) );
    domElement.appendChild(makeElement( domDoc, "MAC", "", "", item->macAddress() ) );
    domElement.appendChild( makePosNod( domDoc, item->center() ) );
    domElement.appendChild( makeConnectsNod( domDoc, item->connects() ) );
    return domElement;
}

QDomElement ElectronicsXmlParser::makeConnectsNod( QDomDocument&    domDoc,
                                        const ElConnMap& elConnMap )
{
    QDomElement connects;
    if( elConnMap.isEmpty() ) return connects;
    connects = domDoc.createElement( "connects" );
    ElConnMap::const_iterator it = elConnMap.begin();
    for( int i = 1; it != elConnMap.end(); i++, it++ ) {

        QDomElement connection = makeElement( domDoc, "connection",
                                           "id", QString::number( i ) );
        connection.appendChild( makeElement( domDoc, "connectoionType", "", "",
                                           QString::number( it.value() ) ) );
        connection.appendChild( makeElement( domDoc, "connectWith", "", "",
                                           it.key()->name() ) );
        connects.appendChild( connection );
    }
    return connects;
}

QDomElement ElectronicsXmlParser::makePosNod( QDomDocument& domDoc,
                                              const Position& pos )
{
    QDomElement position = makeElement( domDoc, "position", "", "", "" );
    position.appendChild( makeElement( domDoc, "x", "", "",
                                       QString::number( pos.x() ) ) );
    position.appendChild( makeElement( domDoc, "y", "", "",
                                         QString::number( pos.y() ) ) );
    return position;
}
