#ifndef BULLS_H
#define BULLS_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class Bulls;
}

class Bulls : public QWidget
{
    Q_OBJECT
    
public:
    explicit Bulls(QWidget *parent = 0, bool is_select = false);
    ~Bulls();
    void print();

signals:
    void id_changed(int new_id);
    
private slots:
    void on_bullAddButton_clicked();

    void on_bullsTable_doubleClicked(const QModelIndex &index);

    void on_bullDelButton_clicked();

    void on_filterButton_clicked();

    void set_filter(QString);

    void bull_added();

#ifndef QT_NO_PRINTER

    void printDocument(QPrinter *printer);

#endif

private:
    bool is_select;
    bool filter;
    Ui::Bulls *ui;
    QSqlRelationalTableModel *model;
};

#endif // BULLS_H
