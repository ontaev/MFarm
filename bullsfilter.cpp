#include "bullsfilter.h"
#include "ui_bullsfilter.h"
#include "bulls.h"

#include <QDebug>

bullsFilter::bullsFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bullsFilter)
{
    ui->setupUi(this);
}

bullsFilter::~bullsFilter()
{
    delete ui;
}

void bullsFilter::on_applyButton_clicked()
{
    QString filter_string;

    QString Number;
    QString Name;
    QString InvNumber;
    QString Mark;
    QString Breed;
    QString Line;
    QString BirthDate;
    QString BirthPlace;
    QString Colour;
    QString Family;
    QString InDate;
    QString Owner;

    Number = (ui->NumberCombo->currentIndex())?(QString("Number %1 %2 AND ").arg(ui->NumberCombo->currentText()).arg(QString::number(ui->NumberTxt->value()))):"";
    Name = (ui->NameCombo->currentIndex())?(QString("Name %1 %2 AND ").arg(ui->NameCombo->currentText()).arg(ui->NameTxt->text())):"";
    InvNumber = (ui->InvCombo->currentIndex())?(QString("InvNumber %1 %2 AND ").arg(ui->InvCombo->currentText()).arg(QString::number(ui->InvNumTxt->value()))):"";
    Mark = (ui->MarkCombo->currentIndex())?(QString("Mark %1 %2 AND ").arg(ui->MarkCombo->currentText()).arg(ui->MarkTxt->text())):"";
    Breed = (ui->BreedCombo->currentIndex())?(QString("Breed %1 %2 AND ").arg(ui->BreedCombo->currentText()).arg(ui->BreedTxt->text())):"";
    Line = (ui->LineCombo->currentIndex())?(QString("Line %1 %2 AND ").arg(ui->LineCombo->currentText()).arg(ui->LineTxt->text())):"";
    BirthDate = (ui->BDateCombo->currentIndex())?(QString("BirthDate %1 %2 AND ").arg(ui->BDateCombo->currentText()).arg(ui->BirthDateTxt->text())):"";
    BirthPlace = (ui->BPlaceCombo->currentIndex())?(QString("BirthPlace %1 %2 AND ").arg(ui->BPlaceCombo->currentText()).arg(ui->BirthPlaceTxt->text())):"";
    Colour = (ui->ColourCombo->currentIndex())?(QString("Colour %1 %2 AND ").arg(ui->ColourCombo->currentText()).arg(ui->ColourTxt->text())):"";
    Family = (ui->FamilyCombo->currentIndex())?(QString("Family %1 %2 AND ").arg(ui->FamilyCombo->currentText()).arg(ui->FamilyTxt->text())):"";
    InDate = (ui->InDateCombo->currentIndex())?(QString("InDate %1 %2 AND ").arg(ui->InDateCombo->currentText()).arg(ui->InDateTxt->text())):"";
    Owner = (ui->OwnerCombo->currentIndex())?(QString("Owner %1 %2 AND ").arg(ui->OwnerCombo->currentText()).arg(ui->OwnerTxt->text())):"";

    filter_string = Number +
            Name +
            InvNumber +
            Mark +
            Breed +
            Line +
            BirthDate +
            BirthPlace +
            Colour +
            Family +
            InDate +
            Owner;

    filter_string.chop(5);

    emit(set_filter(filter_string));
    close();
    qDebug() << "filter is: " << filter_string;


}
