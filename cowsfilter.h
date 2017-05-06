#ifndef COWSFILTER_H
#define COWSFILTER_H

#include <QDialog>

namespace Ui {
class cowsFilter;
}

class cowsFilter : public QDialog
{
    Q_OBJECT
    
public:
    explicit cowsFilter(QWidget *parent = 0);
    ~cowsFilter();

signals:
    void set_filter(QString filter_string);
    
private slots:
    void on_applyButton_clicked();

private:
    Ui::cowsFilter *ui;
};

#endif // COWSFILTER_H
