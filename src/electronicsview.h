#ifndef ELECTRONICSVIEW_H
#define ELECTRONICSVIEW_H

#include <QGraphicsView>
class MainWindow;
class ElectronicsItem;
class Connection;

class ElectronicsView : public QGraphicsView
{
public:
    ElectronicsView( MainWindow* core, QWidget* parent );
    ~ElectronicsView();

    void removeHalfLine();
    bool hasHalfLine() const;
    bool hasSelectedItems() const;

protected:
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
        void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent(QDropEvent *event);

private:
    Connection* conn;
    MainWindow* m_core;
    ElectronicsItem* elMouse;

    bool newConnection;
};


#endif // ELECTRONICSVIEW_H
