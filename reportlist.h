#ifndef REPORTLIST_H
#define REPORTLIST_H

#include <QtGui>
#include <QDialog>

namespace Ui {
class reportList;
}

class reportList : public QDialog
{
    Q_OBJECT
    
public:
    explicit reportList(QWidget *parent = 0);
    ~reportList();
    void report(QString filename);
    
private slots:
    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::reportList *ui;
};

#endif // REPORTLIST_H
