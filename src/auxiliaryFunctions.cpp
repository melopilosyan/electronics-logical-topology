#include "neededTypes.h"


int nameToType( QString name )
{
    if (name == "PC")
        return PC;
    else if (name == "IPhone")
        return IPhone;
    else if (name == "Phone")
        return Phone;
    else if (name ==  "Router")
        return Router;
    else if (name ==  "Server")
        return Server;
    else if (name == "Satellite")
        return Satellite;
    else if (name == "Station")
        return Station;
    else if( name == "Wan" )
        return Wan;
    else if( name == "Lan" )
        return Lan;
    else if( name == "WiFi" )
        return WiFi;
    else
        return NoElectronics;
}

QString typeToName( int type )
{
    switch( type ) {
        case PC: return "PC";
        case Phone: return "Phone";
        case IPhone: return "IPhone";
        case Router: return "Router";
        case Server: return "Server";
        case Station: return "Station";
        case Satellite: return "Satellite";
        case Lan: return "LAN";
        case Wan: return "WAN";
        case WiFi: return "WiFi";
    default: return "None";
    }
    return "None";
}

QPixmap typeToPixmap( int type )
{
    //QString pixPath = QObject::tr(":/images/%1.png")
    //                            .arg( typeToName( type ).toLower());
    QPixmap pix;
    switch( type ) {
        case PC: pix =  QPixmap( ":/images/pc.png" ).scaled( 90, 100 ); break;
        case IPhone: pix = QPixmap( ":/images/iphone.png" ).scaled( 35, 65 ); break;
        case Phone: pix = QPixmap( ":/images/phone.png" ).scaled( 110, 70 ); break;
        case Router: pix = QPixmap( ":/images/router.png" ).scaled( 90, 80 ); break;
        case Server: pix = QPixmap( ":/images/server.png" ); break;
        case Satellite: pix = QPixmap( ":/images/satellite.png" ); break;
        case Station: pix = QPixmap( ":/images/station.png" ); break;
        case Wan: pix = QPixmap( ":/images/wan.png" ); break;
        case Lan: pix = QPixmap( ":/images/lan.png" ); break;
        case WiFi: pix = QPixmap( ":/images/wifi.png" ); break;
    default: break;
    }
    return pix;
}
