#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "bulls.h"
#include "cows.h"
#include "newdb.h"
#include "dblist.h"
#include "reportlist.h"
#include "aboutfarm.h"
#include "dbconnect.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QtGui>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //createConnection();

    ui->setupUi(this);

    status = new QLabel(this);
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    setStatus("Ready");
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mdiArea;
    delete status;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDlg(this);
    aboutDlg.exec();
}

void MainWindow::on_actionMale_triggered()
{
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        QWidget* bulls = new Bulls();
        bulls->setAttribute(Qt::WA_DeleteOnClose);
        mdiArea->addSubWindow(bulls);
        bulls->show();
    }
    else{
        QMessageBox::warning(this, tr("Error"), tr("There is no connection to a database"));
    }


}

void MainWindow::on_actionFemale_triggered()
{
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        QWidget* cows = new Cows();
        cows->setAttribute(Qt::WA_DeleteOnClose);
        mdiArea->addSubWindow(cows);
        cows->show();
    }
    else{
        QMessageBox::warning(this, tr("Error"), tr("There is no connection to a database"));
    }
}

void MainWindow::saveLog(const QString &datastr){

    QFile *logfile = new QFile(QCoreApplication::applicationDirPath()+"/"+"mfarm.log");

    if (!logfile->open(QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Fail "<<logfile->fileName()<<" not open";
        return;
    }
    else{
        logfile->write("\n"+QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit()+" "+QTime::currentTime().toString("hh:mm:ss").toLocal8Bit()+" ");
    }
    if (!logfile->isOpen())
        return;
    logfile->write(datastr.toLocal8Bit());

    delete logfile;
}

void MainWindow::on_actionOpen_Database_triggered()
{
    dbList dbListDlg(this);
    dbListDlg.exec();
}

void MainWindow::on_actionNew_Database_triggered()
{
    newDb newDbDlg(this);
    newDbDlg.exec();
}

void MainWindow::on_actionPrint_triggered()
{
    if(mdiArea->activeSubWindow()){
        Bulls *pdoc = qobject_cast<Bulls*>(mdiArea->activeSubWindow()->widget());
        qDebug() << "ewe ne zhopa";
            if (pdoc) {
                pdoc->print();
            }
            else {
                qDebug() << "zhopa";
                QMessageBox::warning(this, "", "Беда");
            }

    }
    else{
        return;
    }

}


void MainWindow::on_actionOpen_calendar_triggered()
{
    QCalendarWidget *calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);
    calendar->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(calendar);
    calendar->show();
}

void MainWindow::on_actionStandard_triggered()
{
    reportList rListDialog(this);
    rListDialog.exec();
}

void MainWindow::on_actionAbout_farm_triggered()
{
    aboutFarm aFarmDlg(this);
    aFarmDlg.exec();
}

void MainWindow::setStatus(QString message){
    status->setText(message);
    ui->statusBar->addWidget(status);
}
