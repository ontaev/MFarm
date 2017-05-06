#ifndef BULLSFILTER_H
#define BULLSFILTER_H

#include <QDialog>

namespace Ui {
class bullsFilter;
}

class bullsFilter : public QDialog
{
    Q_OBJECT
    
public:
    explicit bullsFilter(QWidget *parent = 0);
    ~bullsFilter();

signals:
    void set_filter(QString filter_string);
    
private slots:
    void on_applyButton_clicked();

private:
    Ui::bullsFilter *ui;
};

#endif // BULLSFILTER_H
