#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "neededTypes.h"

#include <QFileInfoList>
#include <QMainWindow>
#include <QDir>

class ElectronicsView;
class QSignalMapper;
class ElServerConnection;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
    ~MainWindow();


    void setSelElTypeNone();
    void updateConnections();
    void setSelConnTypeNone();
    void addConnection( Connection* conn );
    void addElectronics( ElectronicsItem* item );

    ConnectionType selectedConnection() const;
    ElectronicsType selectedElectronics() const;

protected:
    void keyPressEvent( QKeyEvent* event );

signals:

private slots:
    void open();
    void openFile();
    void save();
    void saveAs();
    void zoomIn();
    void zoomOut();
    void addNewTab();
    void saveToImage();
    void deleteSelectedItems();
    void actionPressed( int id );
    void removeTab( int index = -1 );
    void tabIndexChanged( int index );
    void deleteTabContent( int tab = -1 );

private:
    ElectronicsType selectedEl;
    ConnectionType selectedConn;

    unsigned curTab;
    QTabWidget* viewsTab;
    ElectronicsView* curView;
    QSignalMapper* actionMapper;
    ElServerConnection* serververConnection;

    QAction* newAction;
    QAction* openAction;
    QAction* loadAction;
    QAction* saveAction;
    QAction* clearAction;
    QAction* saveAsAction;
    QAction* saveAsImageAction;
    QAction* deleteAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* curElAction;
    QAction* curConnAction;

    QDir lastSavedDir;
    QDir lastOpenedDir;
    QFileInfoList fileList;
    QList<ElList> elListList;
    QList<ConnList> connListList;

    void createMenus();
    void saveSettings();
    void loadSettings();
    void createItemBar();
    void setWindowTitle();
    void createStatusBar();
    void showElectronics();
    void configureWidgets();
    bool load( const QString& fileName );
    void addAction( QToolBar* itemBar, int id );
    void removeConnectionsWith( ElectronicsItem* item );
    void addActionIntoToolBar( QToolBar* toolBar, int id );
    bool isCouples( ElectronicsItem* item1, ElectronicsItem* item2 );
};

#endif // MAINWINDOW_H
