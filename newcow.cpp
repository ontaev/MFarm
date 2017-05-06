#include "newcow.h"
#include "ui_newcow.h"
#include "cowcard.h"

#include <QtSql>

newCow::newCow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newCow)
{
    ui->setupUi(this);

    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

newCow::~newCow()
{
    delete ui;
}

void newCow::submit()
{
    QSqlQuery query;
    query.prepare("INSERT INTO cows (Number, Name, InvNumber, Mark, Breed, Line,"
                  " Validity, Family, BirthDate, BirthPlace, Purpose, FirstCalving, Colour, BloodGroup, InDate, Owner)"
                  "VALUES (:Number, :Name, :InvNumber, :Mark, :Breed, :Line,"
                  " :Validity, :Family, :BirthDate, :BirthPlace, :Purpose, :FirstCalving, :Colour, :BloodGroup, :InDate, :Owner)");
    query.bindValue(":Number", ui->NumberTxt->value());
    query.bindValue(":Name", ui->NameTxt->text());
    query.bindValue(":InvNumber", ui->InvNumTxt->value());
    query.bindValue(":Mark", ui->MarkTxt->text());
    query.bindValue(":Breed", ui->BreedTxt->text());
    query.bindValue(":Line", ui->LineTxt->text());
    query.bindValue(":Validity", ui->ValidityTxt->text());
    query.bindValue(":Family", ui->FamilyTxt->text());
    query.bindValue(":BirthDate", ui->BirthDateTxt->date());
    query.bindValue(":BirthPlace", ui->BirthPlaceTxt->text());
    query.bindValue(":Purpose", ui->PurposeTxt->text());
    query.bindValue(":FirstCalving", ui->FirstCalvingTxt->text());
    query.bindValue(":Colour", ui->ColourTxt->text());
    query.bindValue(":BloodGroup", ui->BloodGroupTxt->text());
    query.bindValue(":InDate", ui->InDateTxt->date());
    query.bindValue(":Owner", ui->OwnerTxt->text());

    if(query.exec()){
        emit(cow_added());
        int id = query.lastInsertId().toInt();
        cowCard newCard (this, id);
        newCard.exec();
    }
    qDebug() << query.lastError();
}
