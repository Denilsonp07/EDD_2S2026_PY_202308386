#ifndef VISTAADMIN_H
#define VISTAADMIN_H

#include <QDialog>

namespace Ui {
class vistaAdmin;
}

class vistaAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit vistaAdmin(QWidget *parent = nullptr);
    ~vistaAdmin();

private:
    Ui::vistaAdmin *ui;
};

#endif // VISTAADMIN_H
