#ifndef CALVING_H
#define CALVING_H

#include <QDialog>
#include <QWidget>
#include <QtSql>

namespace Ui {
class Calving;
}

class Calving : public QDialog
{
    Q_OBJECT
    
public:
    explicit Calving(QWidget *parent = 0, int insem_id = 0);
    ~Calving();
    
signals:
    void calve_added();

private slots:
    void on_bullButton_toggled(bool checked);

    void on_cowButton_toggled(bool checked);

    void on_AnimalList_doubleClicked(const QModelIndex &index);

private:
    Ui::Calving *ui;
    QSqlRelationalTableModel *bullModel;
    QSqlRelationalTableModel *cowModel;
    int insem_id;

};

#endif // CALVING_H
