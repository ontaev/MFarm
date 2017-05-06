#ifndef NEWBULL_H
#define NEWBULL_H

#include <QDialog>

namespace Ui {
class newBull;
}

class newBull : public QDialog
{
    Q_OBJECT
    
public:
    explicit newBull(QWidget *parent = 0);
    ~newBull();

signals:
    void bull_added();

private slots:
    void submit();
    
private:
    Ui::newBull *ui;
};

#endif // NEWBULL_H
