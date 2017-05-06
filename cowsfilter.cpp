#include "cowsfilter.h"
#include "ui_cowsfilter.h"

#include <QDebug>

cowsFilter::cowsFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cowsFilter)
{
    ui->setupUi(this);
}

cowsFilter::~cowsFilter()
{
    delete ui;
}

void cowsFilter::on_applyButton_clicked()
{
    QString filter_string;

    QString Number;
    QString Name;
    QString InvNumber;
    QString Mark;
    QString Breed;
    QString Line;
    QString Validity;
    QString Family;
    QString BirthDate;
    QString BirthPlace;
    QString Purpose;
    QString FirstCalving;
    QString Colour;
    QString BloodGroup;
    QString InDate;
    QString Owner;

    Number = (ui->NumberCombo->currentIndex())?(QString("Number %1 %2 AND ").arg(ui->NumberCombo->currentText()).arg(QString::number(ui->NumberTxt->value()))):"";
    Name = (ui->NameCombo->currentIndex())?(QString("Name %1 %2 AND ").arg(ui->NameCombo->currentText()).arg(ui->NameTxt->text())):"";
    InvNumber = (ui->InvCombo->currentIndex())?(QString("InvNumber %1 %2 AND ").arg(ui->InvCombo->currentText()).arg(QString::number(ui->InvNumTxt->value()))):"";
    Mark = (ui->MarkCombo->currentIndex())?(QString("Mark %1 %2 AND ").arg(ui->MarkCombo->currentText()).arg(ui->MarkTxt->text())):"";
    Breed = (ui->BreedCombo->currentIndex())?(QString("Breed %1 %2 AND ").arg(ui->BreedCombo->currentText()).arg(ui->BreedTxt->text())):"";
    Line = (ui->LineCombo->currentIndex())?(QString("Line %1 %2 AND ").arg(ui->LineCombo->currentText()).arg(ui->LineTxt->text())):"";
    Validity = (ui->ValidityCombo->currentIndex())?(QString("Validity %1 %2 AND ").arg(ui->ValidityCombo->currentText()).arg(ui->ValidityTxt->text())):"";
    Family = (ui->FamilyCombo->currentIndex())?(QString("Family %1 %2 AND ").arg(ui->FamilyCombo->currentText()).arg(ui->FamilyTxt->text())):"";
    BirthDate = (ui->BDateCombo->currentIndex())?(QString("BirthDate %1 %2 AND ").arg(ui->BDateCombo->currentText()).arg(ui->BirthDateTxt->text())):"";
    BirthPlace = (ui->BPlaceCombo->currentIndex())?(QString("BirthPlace %1 %2 AND ").arg(ui->BPlaceCombo->currentText()).arg(ui->BirthPlaceTxt->text())):"";
    Purpose = (ui->PurposeCombo->currentIndex())?(QString("Purpose %1 %2 AND ").arg(ui->PurposeCombo->currentText()).arg(ui->PurposeTxt->text())):"";
    FirstCalving = (ui->FCalvingCombo->currentIndex())?(QString("FirstCalving %1 %2 AND ").arg(ui->FCalvingCombo->currentText()).arg(ui->FirstCalvingTxt->text())):"";
    Colour = (ui->ColourCombo->currentIndex())?(QString("Colour %1 %2 AND ").arg(ui->ColourCombo->currentText()).arg(ui->ColourTxt->text())):"";
    BloodGroup = (ui->BGroupCombo->currentIndex())?(QString("BloodGroup %1 %2 AND ").arg(ui->BGroupCombo->currentText()).arg(ui->BloodGroupTxt->text())):"";
    InDate = (ui->InDateCombo->currentIndex())?(QString("InDate %1 %2 AND ").arg(ui->InDateCombo->currentText()).arg(ui->InDateTxt->text())):"";
    Owner = (ui->OwnerCombo->currentIndex())?(QString("Owner %1 %2 AND ").arg(ui->OwnerCombo->currentText()).arg(ui->OwnerTxt->text())):"";

    filter_string = Number +
            Name +
            InvNumber +
            Mark +
            Breed +
            Line +
            Validity +
            Family +
            BirthDate +
            BirthPlace +
            Purpose +
            FirstCalving +
            Colour +
            BloodGroup +
            InDate +
            Owner;

    filter_string.chop(5);

    emit(set_filter(filter_string));
    close();
    qDebug() << "filter is: " << filter_string;

}
