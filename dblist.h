#ifndef DBLIST_H
#define DBLIST_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class dbList;
}

class dbList : public QDialog
{
    Q_OBJECT
    
public:
    explicit dbList(QWidget *parent = 0);
    ~dbList();
    
private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::dbList *ui;
    void updateList();
};

#endif // DBLIST_H
