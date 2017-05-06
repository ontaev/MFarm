#include "newbull.h"
#include "ui_newbull.h"
#include "bullcard.h"

#include <QtSql>


newBull::newBull(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newBull)
{
    ui->setupUi(this);

    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

newBull::~newBull()
{
    delete ui;
}

void newBull::submit()
{
    QSqlQuery query;
    query.prepare("INSERT INTO bulls (Number, Name, InvNumber, Mark, Breed, Line,"
                  " BirthDate, BirthPlace, Colour, Family, InDate, Owner)"
                  "VALUES (:Number, :Name, :InvNumber, :Mark, :Breed, :Line,"
                  " :BirthDate, :BirthPlace, :Colour, :Family, :InDate, :Owner)");
    query.bindValue(":Number", ui->NumberTxt->value());
    query.bindValue(":Name", ui->NameTxt->text());
    query.bindValue(":InvNumber", ui->InvNumTxt->value());
    query.bindValue(":Mark", ui->MarkTxt->text());
    query.bindValue(":Breed", ui->BreedTxt->text());
    query.bindValue(":Line", ui->LineTxt->text());
    query.bindValue(":BirthDate", ui->BirthDateTxt->date());
    query.bindValue(":BirthPlace", ui->BirthPlaceTxt->text());
    query.bindValue(":Colour", ui->ColourTxt->text());
    query.bindValue(":Family", ui->FamilyTxt->text());
    query.bindValue(":InDate", ui->InDateTxt->date());
    query.bindValue(":Owner", ui->OwnerTxt->text());

    if(query.exec()){
        emit(bull_added());
        int id = query.lastInsertId().toInt();
        bullCard newCard (this, id);
        newCard.exec();

    }
    qDebug() << query.lastError();
}
