#ifndef NEWDB_H
#define NEWDB_H

#include <QDialog>

namespace Ui {
class newDb;
}

class newDb : public QDialog
{
    Q_OBJECT
    
public:
    explicit newDb(QWidget *parent = 0);
    ~newDb();
    
private slots:
    void on_createButton_clicked();

    void on_browseButton_clicked();

private:
    Ui::newDb *ui;
};

#endif // NEWDB_H
