#ifndef PHOTODIALOG_H
#define PHOTODIALOG_H

#include <QDialog>
#include <QLabel>

#include <QtSql>
#include <QSqlDatabase>

namespace Ui {
class PhotoDialog;
}

class PhotoDialog : public QDialog
{
    Q_OBJECT

protected:
     void resizeEvent(QResizeEvent *event);

public:
    explicit PhotoDialog(QWidget *parent = 0, int id = 0, int gender = 0);
    void updateScreenshotLabel();
    ~PhotoDialog();
    
private slots:

    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::PhotoDialog *ui;
    QPixmap pixmap;
    QSqlQuery query;
    int id;
    int gender;

    //QLabel *imageLabel;
};

#endif // PHOTODIALOG_H
