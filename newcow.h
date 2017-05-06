#ifndef NEWCOW_H
#define NEWCOW_H

#include <QDialog>

namespace Ui {
class newCow;
}

class newCow : public QDialog
{
    Q_OBJECT
    
public:
    explicit newCow(QWidget *parent = 0);
    ~newCow();

signals:
    void cow_added();

private slots:
    void submit();
    
private:
    Ui::newCow *ui;
};

#endif // NEWCOW_H
