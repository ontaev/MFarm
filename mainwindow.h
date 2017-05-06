#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static void saveLog(const QString&);

    void setStatus(QString);
    
private slots:
    void on_actionAbout_triggered();

    void on_actionMale_triggered();

    void on_actionFemale_triggered();

    void on_actionOpen_Database_triggered();

    void on_actionNew_Database_triggered();

    void on_actionPrint_triggered();

//#ifndef QT_NO_PRINTER
//    void printDocument(QPrinter *printer);

//#endif

    void on_actionOpen_calendar_triggered();

    void on_actionStandard_triggered();

    void on_actionAbout_farm_triggered();

private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
    QLabel *status;

};

#endif // MAINWINDOW_H
