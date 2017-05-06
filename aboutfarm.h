#ifndef ABOUTFARM_H
#define ABOUTFARM_H

#include <QDialog>

namespace Ui {
class aboutFarm;
}

class aboutFarm : public QDialog
{
    Q_OBJECT

private slots:
    void submit();

public:
    explicit aboutFarm(QWidget *parent = 0);
    ~aboutFarm();
    
private:
    Ui::aboutFarm *ui;
};

#endif // ABOUTFARM_H
