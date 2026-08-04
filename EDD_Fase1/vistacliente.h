#ifndef VISTACLIENTE_H
#define VISTACLIENTE_H

#include <QDialog>

namespace Ui {
class vistaCliente;
}

class vistaCliente : public QDialog
{
    Q_OBJECT

public:
    explicit vistaCliente(QWidget *parent = nullptr);
    ~vistaCliente();

private:
    Ui::vistaCliente *ui;
};

#endif // VISTACLIENTE_H
