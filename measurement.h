#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QDialog>

namespace Ui {
class Measurement;
}

class Measurement : public QDialog
{
    Q_OBJECT
    
public:
    explicit Measurement(QWidget *parent = 0, int id = 0, int gender = 2);
    ~Measurement();

signals:
    void meas_added();

private slots:
    void submit();
    
private:
    Ui::Measurement *ui;
    int id;
    int gender;
};

#endif // MEASUREMENT_H
