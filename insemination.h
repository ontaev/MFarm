#ifndef INSEMINATION_H
#define INSEMINATION_H

#include <QDialog>

namespace Ui {
class insemination;
}

class insemination : public QDialog
{
    Q_OBJECT

private slots:
    void submit();
    
public:
    explicit insemination(QWidget *parent = 0, int id = 0);
    ~insemination();

signals:
    void insem_added();
    
private:
    Ui::insemination *ui;
    int id;
};

#endif // INSEMINATION_H
