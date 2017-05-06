#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include "newdb.h"
#include "ui_newdb.h"
#include <QtSql>
#include <QSqlDatabase>

newDb::newDb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newDb)
{
    ui->setupUi(this);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

newDb::~newDb()
{
    delete ui;
}

void newDb::on_createButton_clicked()
{
    const QString conn = QLatin1String("dbcreation");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", conn);
    db.setHostName(ui->ServerTxt->text());
    db.setDatabaseName("mysql");
    db.setUserName("root");
    db.setPassword("root");

    if(db.open()){
        QSqlQuery query(QSqlDatabase::database("dbcreation"));
        query.prepare("CREATE DATABASE "+ui->NameTxt->text());
        if(query.exec()){
            QSettings settings("Bdm Soft", "MFarm");
            settings.beginGroup("Databases");
                settings.beginGroup(ui->NameTxt->text());
                    settings.setValue("Server", ui->ServerTxt->text());
                    settings.setValue("Image folder", ui->ImageTxt->text());
                settings.endGroup();
            settings.endGroup();
            QMessageBox::information(this, tr("New database"), tr("Database %1 succesfully created").arg(ui->NameTxt->text()));

        }
        else{
            QMessageBox::warning(this, tr("New database"), tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        }
        db.close();
    }
    else{
        QMessageBox::warning(this, tr("New database"), tr("The database reported an error: %1").arg(db.lastError().databaseText()));
    }
    QSqlDatabase::removeDatabase("dbcreation");
}

void newDb::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QCoreApplication::applicationDirPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->ImageTxt->setText(dir);
}
