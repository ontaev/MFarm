#include "aboutfarm.h"
#include "ui_aboutfarm.h"

#include <QtGui>
#include <QtSql>

aboutFarm::aboutFarm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutFarm)
{
    ui->setupUi(this);

    QSqlQuery query("SELECT * FROM info WHERE id = 1");


    if (query.exec()){
        if (query.size()){
            query.first();
            ui->NameTxt->setText(query.value(1).toString());
            ui->PostCodeTxt->setText(query.value(2).toString());
            ui->RegionTxt->setText(query.value(3).toString());
            ui->DistrictTxt->setText(query.value(4).toString());
            ui->CityTxt->setText(query.value(5).toString());
            ui->PhoneTxt->setText(query.value(6).toString());

        }
    }
    else{

        QMessageBox::warning(this, tr("About farm"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));

    }

    connect(ui->closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
}

aboutFarm::~aboutFarm()
{
    delete ui;
}

void aboutFarm::submit()
{
    QSqlQuery query("SELECT * FROM info WHERE id = 1");


    if (query.exec()){
        if (query.size()){
            QSqlQuery update;
            update.prepare("UPDATE info "
                             "SET Name = :Name, "
                             "PostCode = :PostCode, "
                             "Region = :Region, "
                             "District = :District, "
                             "City = :City, "
                             "Phone = :Phone "
                             "WHERE id = 1");

            update.bindValue(":Name", ui->NameTxt->text());
            update.bindValue(":PostCode", ui->PostCodeTxt->text());
            update.bindValue(":Region", ui->RegionTxt->text());
            update.bindValue(":District", ui->DistrictTxt->text());
            update.bindValue(":City", ui->CityTxt->text());
            update.bindValue(":Phone", ui->PhoneTxt->text());

            if(!update.exec()){
                QMessageBox::warning(this, tr("About farm"),tr("The database reported an error: %1").arg(update.lastError().databaseText()));
            }

        }
        else{
            QSqlQuery insert;
            insert.prepare("INSERT INTO info (id, Name, PostCode, Region, District, City, Phone)"
                           "VALUES (1, :Name, :PostCode, :Region, :District, :City, :Phone)");

            insert.bindValue(":Name", ui->NameTxt->text());
            insert.bindValue(":PostCode", ui->PostCodeTxt->text());
            insert.bindValue(":Region", ui->RegionTxt->text());
            insert.bindValue(":District", ui->DistrictTxt->text());
            insert.bindValue(":City", ui->CityTxt->text());
            insert.bindValue(":Phone", ui->PhoneTxt->text());

            if(!insert.exec()){
                QMessageBox::warning(this, tr("About farm"),tr("The database reported an error: %1").arg(insert.lastError().databaseText()));
            }

        }

    }
    else{

        QMessageBox::warning(this, tr("About farm"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));

    }
}
