#include "electronicsitemtoolbutton.h"
#include "electronicsxmlparser.h"
#include "elserverconnection.h"
#include "electronicsitem.h"
#include "electronicsview.h"
#include "connection.h"
#include "mainwindow.h"

#include <QSignalMapper>
#include <QFileDialog>
#include <QStatusBar>
#include <QSettings>
#include <QKeyEvent>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>


MainWindow::MainWindow():
    QMainWindow( 0 ),
    selectedEl( NoElectronics ),
    selectedConn( NoConnection ),
    curTab( 0 ),
    viewsTab( new QTabWidget( this ) ),
    curView( 0 ),
    actionMapper( new QSignalMapper( this ) ),
    serververConnection( new ElServerConnection( this ) ),
    newAction( 0 ),
    openAction( 0 ),
    loadAction( 0 ),
    saveAction( 0 ),
    clearAction( 0 ),
    saveAsAction( 0 ),
    saveAsImageAction( 0 ),
    deleteAction( 0 ),
    zoomInAction( 0 ),
    zoomOutAction( 0 ),
    curElAction( 0 ),
    curConnAction( 0 ),
    lastSavedDir(),
    lastOpenedDir(),
    fileList(),
    elListList(),
    connListList()
{
    createMenus();
    loadSettings();
    createItemBar();
    setWindowTitle();
    createStatusBar();
    configureWidgets();
    addNewTab();
    tabIndexChanged( 0 );
    connect( viewsTab, SIGNAL( tabCloseRequested( int ) ), SLOT( removeTab( int ) ) );
    connect( actionMapper, SIGNAL( mapped( int ) ), this, SLOT( actionPressed (int ) ) );
    connect( viewsTab, SIGNAL( currentChanged( int ) ), SLOT( tabIndexChanged( int ) ) );
}

MainWindow::~MainWindow()
{
    saveSettings();
    unsigned tabNum = viewsTab->count();
    for( curTab = 0; curTab < tabNum; curTab++ )
        deleteTabContent();
}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
    switch( event->key() ) {
        case Qt::Key_Escape:
            if( curView->hasHalfLine() ) {
                curView->removeHalfLine();
            } else if( selectedEl != NoElectronics ) {
                selectedEl = NoElectronics;
                curElAction->setChecked( false );
                curElAction = 0;
            } else if( selectedConn != NoConnection ) {
                selectedConn = NoConnection;
                curConnAction->setChecked( false );
                curConnAction = 0;
            }
            break;
        default: break;
    }
    QMainWindow::keyPressEvent( event );
}

void MainWindow::actionPressed( int id )
{
    QAction* action = ( QAction* )actionMapper->mapping( id );
    if( id < 10 ) {
        if( selectedConn == id ) {
            selectedConn = NoConnection;
            action->setChecked( false );
        } else {
            selectedConn = ( ConnectionType )id;
            if( curConnAction ) curConnAction->setChecked( false );
            action->setChecked( true );
        }
        curConnAction = action;
    } else {
        if( selectedEl == id ) {
            selectedEl = NoElectronics;
            action->setChecked( false );
        } else {
            if( curElAction ) curElAction->setChecked( false );
            selectedEl = ( ElectronicsType )id;
            action->setChecked( true );
        }
        curElAction = action;
    }
}

void MainWindow::tabIndexChanged( int index )
{
    curTab = index;
    curView = ( ElectronicsView* )viewsTab->widget( index );
    setWindowTitle();
}

void MainWindow::deleteSelectedItems()
{
    QList<QGraphicsItem*> items = curView->scene()->selectedItems();
    if( !items.isEmpty() ) {
        foreach( QGraphicsItem* item, items ) {
            if( qgraphicsitem_cast< ElectronicsItem* >( item ) ) {
                ElectronicsItem* elItem = ( ElectronicsItem* )item;
                removeConnectionsWith( elItem );
                elItem->disconnect();
                curView->scene()->removeItem( elItem );
                elListList[curTab].removeOne( elItem );
                delete elItem;
            }
        }
    }
    items = curView->scene()->selectedItems();
    if( !items.isEmpty() ) {
        foreach( QGraphicsItem* item, items ) {
            if( qgraphicsitem_cast< Connection* >( item ) ) {
                Connection* conn = ( Connection* )item;
                conn->disconnectLimbs();
                curView->scene()->removeItem( conn );
                connListList[curTab].removeOne( conn );
                delete conn;
            }
        }
    }
}

void MainWindow::save()
{
    if( fileList[curTab].fileName().isEmpty() ) {
        saveAs();
    } else {
        ElectronicsXmlParser::write( fileList[curTab].filePath(), elListList[curTab] );
        viewsTab->setTabText( curTab, fileList[curTab].fileName() );
        statusBar()->showMessage( "File saved.", 4000 );
        setWindowTitle();
    }
}

QString setSuffix( QString path, const QString& suf )
{
    return path.remove( QRegExp( ".(xml|png|jpg)$" ) ) + "." + suf;
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName( this, "Save As .xml File",
                                            lastSavedDir.path(), "XML files ( *.xml )" );
    if( !fileName.isEmpty() ) {
        fileList[curTab].setFile( setSuffix( fileName, "xml" ) );
        lastSavedDir = fileList[curTab].dir();
        ElectronicsXmlParser::write( fileList[curTab].filePath(), elListList[curTab] );
        statusBar()->showMessage( "Saved as "+ fileList[curTab].fileName(), 4000 );
        viewsTab->setTabText( curTab, fileList[curTab].fileName() );
        setWindowTitle();
    }
}

void MainWindow::saveToImage()
{
    QString fileName = QFileDialog::getSaveFileName( this,
                            "Save As Image", "", "Images (*.png *.jpg)" );
    QImage image( curView->size(), QImage::Format_RGB32 );
    QPainter painter( &image );
    curView->render( &painter );
    QString filePath = setSuffix( fileName, "png" );
    image.save( filePath, "PNG", 9 );
    statusBar()->showMessage( "Saved as " + QFileInfo( filePath ).fileName(), 4000 );
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Choose an xml file to load",
                                                  lastOpenedDir.path(), "XML files ( *.xml )" );

    if( !fileName.isEmpty() ) {
        load( fileName );
    }
}
void MainWindow::openFile()
{
    QString fileName = serververConnection->getFile();
    load( fileName );
}



bool MainWindow::load( const QString& fileName )
{
    deleteTabContent();
    fileList[curTab].setFile( fileName );
    lastOpenedDir = fileList[curTab].dir();
    bool read = ElectronicsXmlParser::read( fileName, elListList[curTab] );
    if( read ) {
        setWindowTitle();
        showElectronics();
        viewsTab->setTabText( curTab, fileList[curTab].fileName() );
        statusBar()->showMessage( tr("File \"%1\" opened")
                                  .arg( fileList[curTab].fileName() ), 4000 );
        return true;
    }
    return false;
}

void MainWindow::zoomIn()
{
    curView->scale( 1.1, 1.1 );
}

void MainWindow::zoomOut()
{
    curView->scale( 0.9, 0.9 );
}

void MainWindow::showElectronics()
{
    if( elListList[curTab].isEmpty() ) return;
    foreach( ElectronicsItem* item, elListList[curTab] ) {
        curView->scene()->addItem( item );
        ElConnMap connects = item->connects();
        if( connects.isEmpty() ) continue;
        ElConnMap::const_iterator it = connects.begin();
        for( ; it != connects.end(); it++ ) {
            if( isCouples( it.key(), item ) ) continue;
            Connection* conn = new Connection( item, it.key(),
                                               it.value(), curView->scene() );
            conn->connectLimbs();
            connListList[curTab] << conn;
        }
    }
}

void MainWindow::removeConnectionsWith( ElectronicsItem* item )
{
    ConnList& connList = connListList[curTab];
    if( connList.isEmpty() ) return;
    int sz = connList.size();
    for( int i = 0; i < sz; i++ ) {
        if( connList[i]->isLimb( item ) ) {
            curView->scene()->removeItem( connList[i] );
            connList[i]->disconnectLimbs();
            delete connList[i];
            connList.removeAt( i );
            sz--; i--;
        }
    }
}

bool MainWindow::isCouples( ElectronicsItem* item1, ElectronicsItem* item2 )
{
    ConnList& connList = connListList[curTab];
    if( connList.isEmpty() ) return false;
    foreach( Connection* conn, connList ) {
        if( conn->isLimbs( item1, item2 ) )
            return true;
    }
    return false;
}

void MainWindow::updateConnections()
{
    ConnList& connList = connListList[curTab];
    if( connList.isEmpty() ) return;
    foreach( Connection* conn, connList ) {
        conn->updateLine();
    }
}

void MainWindow::removeTab( int index )
{
    if( viewsTab->count() > 1 ) {
        if( index == -1 ) index = curTab;
        deleteTabContent();
        fileList.removeAt( index );
        viewsTab->removeTab( index );
        elListList.removeAt( index );
        connListList.removeAt( index );
        setWindowTitle();
    }
}

void MainWindow::addNewTab()
{
    int index = viewsTab->addTab( new ElectronicsView( this, viewsTab ), "Untitled" );
    viewsTab->setCurrentIndex( index );
    fileList << QFileInfo();
    elListList << ElList();
    connListList << ConnList();
}

void MainWindow::createItemBar()
{
    QToolBar* itemBar = addToolBar( "ElConnItems" );
    addToolBar( Qt::LeftToolBarArea, itemBar );
    itemBar->setIconSize( QSize( 50, 50 ) );
    itemBar->setMovable( false );

    QVector<int> ids;
    ids << IPhone << PC << Server << Station << Router << Phone << Satellite;
    foreach ( int id, ids )
		addActionIntoToolBar( itemBar, id );
    itemBar->addSeparator();
    ids.clear();
    ids  << Wan<< Lan << WiFi;
    foreach ( int id, ids )
		addActionIntoToolBar( itemBar, id );
}

void MainWindow::createMenus()
{
    QList<QAction*> separatorList;

    QMenu* file = menuBar()->addMenu( "&File" );

    newAction = file->addAction( QIcon(":/menu/new"), "&New project",
                                 this, SLOT( addNewTab() ), QKeySequence::New );
    newAction->setStatusTip( "New project." );

    openAction = file->addAction( QIcon(":/menu/open"), "Open &xml file",
                                this, SLOT( open() ), QKeySequence::Open );
    openAction->setStatusTip( "Open existing project." );

    loadAction = file->addAction(QIcon(":/menu/load"),"Download &xml file",
                                  this,SLOT( openFile()), QKeySequence("Ctrl+L") );
    loadAction->setStatusTip("Download xml file");


    separatorList << file->addSeparator();

    saveAction = file->addAction( QIcon(":/menu/save"), "&Save",
                                this, SLOT( save() ), QKeySequence::Save );
    saveAction->setStatusTip( "Save project." );
    saveAsAction = file->addAction( QIcon(":/menu/saveas"), "Save &As...",
                                this, SLOT( saveAs() ), QKeySequence::SaveAs );
    saveAsAction->setStatusTip( "Save as new project." );
    saveAsImageAction = file->addAction( QIcon(":/menu/savetoimage"), "Save As &image",
                                this, SLOT( saveToImage() ), QKeySequence::Print );
    saveAsImageAction->setStatusTip( "Save as png file." );
    separatorList << file->addSeparator();
    file->addAction( "Close &project", this, SLOT( removeTab() ), QKeySequence( "Ctrl+W" ))
    ->setStatusTip( "Close current project" );
    clearAction = file->addAction( QIcon(":/menu/clear"), "Clear &project",
                                  this, SLOT( deleteTabContent() ), QKeySequence( "Ctrl+Shift+X" ) );
    clearAction->setStatusTip( "Clear current project" );
    separatorList << file->addSeparator();
    file->addAction( "&Quit", this, SLOT( close() ), QKeySequence::Quit )
    ->setStatusTip( "Quit window" );

    QMenu* edit = menuBar()->addMenu( "&Edit" );
    deleteAction = edit->addAction( QIcon(":/menu/delete"), "Remove &item(s)",
                                this, SLOT( deleteSelectedItems() ), QKeySequence::Delete );
    deleteAction->setStatusTip( "Remove selected item(s)" );

    QMenu* view = menuBar()->addMenu( "&View" );
    zoomInAction = view->addAction( QIcon(":/menu/zoomin"), "Zoom &In",
                                this, SLOT( zoomIn() ), QKeySequence::ZoomIn );
    zoomInAction->setStatusTip( "Zoom in." );
    zoomOutAction = view->addAction( QIcon(":/menu/zoomout"), "Zoom &Out",
                                this, SLOT( zoomOut() ), QKeySequence::ZoomOut );
    zoomOutAction->setStatusTip( "Zoom out." );


    QList<QAction *> actionList;
    actionList << newAction << openAction << loadAction<< separatorList[0]
               << saveAction << saveAsAction << saveAsImageAction
               << separatorList[1] << deleteAction << clearAction
               << separatorList[2] << zoomInAction << zoomOutAction;
    addToolBar( "MenuBar" )->addActions( actionList );
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue( "LastOpenedDir", lastOpenedDir.path() );
    settings.setValue( "LastSavedDir", lastSavedDir.path() );
    settings.setValue( "Size", size() );
}

void MainWindow::loadSettings()
{
    QSettings settings;
    QSize size = settings.value( "Size", QSize( 1100, 750 ) ).toSize();
    lastSavedDir.cd( settings.value( "LastSavedDir", "/home" ).toString() );
    lastOpenedDir.cd( settings.value( "LastOpenedDir", "/home" ).toString() );
    setGeometry( 0, 0, size.width(), size.height() );
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage( "" );
}

void MainWindow::configureWidgets()
{
    setCentralWidget( viewsTab );
    viewsTab->setTabsClosable( true );
    viewsTab->setTabPosition( QTabWidget::South );
}

void MainWindow::deleteTabContent( int tab )
{
    if( tab == -1 ) tab = curTab;
    foreach( ElectronicsItem* item, elListList[tab] ) {
        if( item ) {
            if( item->scene() ) curView->scene()->removeItem( item );
            delete item;
        }
    }
    elListList[tab].clear();
    foreach( Connection* conn, connListList[tab] ) {
        if( conn ) {
            if( conn->scene() ) curView->scene()->removeItem( conn );
            delete conn;
        }
    }
    connListList[tab].clear();
}

void MainWindow::addActionIntoToolBar( QToolBar* toolBar, int id )
{
    QAction *action = new QAction( QIcon( typeToPixmap( id ) ),
                                   typeToName( id ), toolBar );
    if( 0 ) { // For convenience
        ElectronicsToolButton *toolButton = new ElectronicsToolButton;
        toolButton->setDefaultAction( action );
        toolBar->addWidget( toolButton );
    } else toolBar->addAction( action );
    action->setCheckable( true );
    actionMapper->setMapping( action, id );
    connect( action, SIGNAL( triggered() ), actionMapper, SLOT( map() ) );
    action->setStatusTip( typeToName( id ) + ( (id < 10) ? " connection": "") );
}

void MainWindow::setWindowTitle()
{
    QString title( "Electronics Logical Topology" );
    if( !fileList.value( curTab ).fileName().isEmpty() )
            title += ( fileList.value( curTab ).fileName().isEmpty() ) ? "":
                           tr(" - %1").arg( fileList.value( curTab ).fileName() );
    QMainWindow::setWindowTitle( title );
}

void MainWindow::setSelElTypeNone()
{
    selectedEl = NoElectronics;
    curElAction->setChecked( false );
    curElAction = 0;
}

void MainWindow::setSelConnTypeNone()
{
    selectedConn = NoConnection;
    curConnAction->setChecked( false );
    curConnAction = 0;
}

ElectronicsType MainWindow::selectedElectronics() const
{   return selectedEl;   }

ConnectionType MainWindow::selectedConnection() const
{   return selectedConn;   }

void MainWindow::addConnection( Connection* conn )
{   connListList[curTab].append( conn );   }

void MainWindow::addElectronics( ElectronicsItem* item )
{   elListList[curTab].append( item );  }


