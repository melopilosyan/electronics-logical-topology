#ifndef ELECTRONICSITEMTOOLBUTTON_H
#define ELECTRONICSITEMTOOLBUTTON_H

#include <QToolButton>
class ElectronicsItem;

class ElectronicsToolButton : public QToolButton
{
public:
    ElectronicsToolButton();
    ElectronicsToolButton(QWidget *parent);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *e);
private:
    QPoint m_ptDragPos;
};

#endif // ELECTRONICSITEMTOOLBUTTON_H
