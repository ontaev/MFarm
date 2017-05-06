#ifndef ADDDB_H
#define ADDDB_H

#include <QDialog>

namespace Ui {
class addDb;
}

class addDb : public QDialog
{
    Q_OBJECT
    
public:
    explicit addDb(QWidget *parent = 0);
    ~addDb();
    
private slots:
    void on_browseButton_clicked();

    void on_checkButton_clicked();

    void on_NameTxt_textChanged(const QString &arg1);

    void on_ServerTxt_textChanged(const QString &arg1);

private:
    Ui::addDb *ui;
    bool checked;
};

#endif // ADDDB_H
