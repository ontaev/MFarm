#ifndef COWS_H
#define COWS_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class Cows;
}

class Cows : public QWidget
{
    Q_OBJECT
    
public:
    explicit Cows(QWidget *parent = 0,  bool is_select = false);
    ~Cows();
	void print();

signals:
    void id_changed(int new_id);
    
private slots:
    void on_cowAddButton_clicked();

    void on_cowsTable_doubleClicked(const QModelIndex &index);

    void on_cowDelButton_clicked();

    void on_filterButton_clicked();

    void set_filter(QString);

    void cow_added();

#ifndef QT_NO_PRINTER

    void printDocument(QPrinter *printer);

#endif

private:
    bool is_select;
    bool filter;
    Ui::Cows *ui;
    QSqlRelationalTableModel *model;
};

#endif // COWS_H
