#ifndef ELINITDIALOG_H
#define ELINITDIALOG_H

#include <QDialog>
class QLineEdit;
class ElectronicsItem;

class ElInitDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ElInitDialog( ElectronicsItem* el, QWidget* parent = 0 );
    ~ElInitDialog();

private slots:
    void collectData();

private:
    ElectronicsItem* electronics;
    QLineEdit* nameEdit;
    QLineEdit* ipEdit;
    QLineEdit* macEdit;

    void createLabels();
};

#endif // ELINITDIALOG_H
