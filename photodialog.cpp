#include "photodialog.h"
#include "ui_photodialog.h"

#include <QtGui>
#include <QFileDialog>


PhotoDialog::PhotoDialog(QWidget *parent, int id, int gender) :
    QDialog(parent),
    ui(new Ui::PhotoDialog)
{
    ui->setupUi(this);
    ui->imageLabel->setMinimumSize(440, 330);
    ui->imageLabel->setAlignment(Qt::AlignHCenter);

    PhotoDialog::id = id;
    PhotoDialog::gender = gender;
    //QImage image("D:/QtProjects/MFarm/Photo/1.jpg");
    //pixmap = QPixmap::fromImage(image);
    //updateScreenshotLabel();

    //QSqlQuery query;
    query.prepare("SELECT idphoto, file FROM photo WHERE id_animal = :ID AND GENDER = :GENDER");
    query.bindValue(":ID", id);
    query.bindValue(":GENDER", gender);


    if(query.exec()){
        query.first();
        QString file_path = query.value(1).toString();
        qDebug() << file_path;
        QImage image(file_path);
        pixmap = QPixmap::fromImage(image);
        updateScreenshotLabel();
    }

}

PhotoDialog::~PhotoDialog()
{
    delete ui;

}

void PhotoDialog::resizeEvent(QResizeEvent * /* event */)
 {
     QSize scaledSize = pixmap.size();
     scaledSize.scale(ui->imageLabel->size(), Qt::KeepAspectRatio);
     if (!ui->imageLabel->pixmap()
             || scaledSize != ui->imageLabel->pixmap()->size())
         updateScreenshotLabel();
 }
void PhotoDialog::updateScreenshotLabel()
{
    ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}

void PhotoDialog::on_previousButton_clicked()
{
    if(query.previous()){

        QString file_path = query.value(1).toString();
        qDebug() << query.at();
        QImage image(file_path);
        pixmap = QPixmap::fromImage(image);
        updateScreenshotLabel();
    }
    else{
        if(query.at() == QSql::BeforeFirstRow) query.next();
    }
}



void PhotoDialog::on_nextButton_clicked()
{
    if(query.next()){

        QString file_path = query.value(1).toString();
        qDebug() << query.at();
        QImage image(file_path);
        pixmap = QPixmap::fromImage(image);
        updateScreenshotLabel();
    }
    else{
        if(query.at() == QSql::AfterLastRow) query.previous();
    }
}



void PhotoDialog::on_addButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image"),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Image Files (*.png *.jpg *.bmp)") );
    qDebug() << fileName;
    QFile file(fileName);
    //QString newName = QCoreApplication::applicationDirPath()+"/11111.jpg";
    QString newName = qApp->property("ImagePath").toString()+"/"+
            QDate::currentDate().toString("ddMMyyyy").toLocal8Bit()+
            QTime::currentTime().toString("hhmmss").toLocal8Bit();
    file.copy(newName);

    QSqlQuery add;
    add.prepare("INSERT INTO photo (file, id_animal, gender)"
                  "VALUES (:FILE, :ID, :GENDER)");
    add.bindValue(":FILE", newName);
    add.bindValue(":ID", id);
    add.bindValue(":GENDER", gender);
    if(!add.exec()){
        QMessageBox::warning(this, tr("Add photo"),
                             tr("The database reported an error: %1")
                             .arg(add.lastError().databaseText()));
    }
    else{
        if(query.exec()){
            query.first();
            QString file_path = query.value(1).toString();
            qDebug() << file_path;
            QImage image(file_path);
            pixmap = QPixmap::fromImage(image);
            updateScreenshotLabel();
        }
    }
    //qDebug() << add.lastError();


}

void PhotoDialog::on_deleteButton_clicked()
{
    int idphoto = query.value(0).toInt();
    QSqlQuery delQuery;
    delQuery.prepare("DELETE FROM photo WHERE idphoto = :ID");
    delQuery.bindValue(":ID",idphoto);
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;


    if(!delQuery.exec()){
        QMessageBox::warning(this, tr("Removing data"),
                             tr("The database reported an error: %1")
                             .arg(delQuery.lastError().databaseText()));

    }
    else{
        if(query.exec()){
            query.first();
            QString file_path = query.value(1).toString();
            qDebug() << file_path;
            QImage image(file_path);
            pixmap = QPixmap::fromImage(image);
            updateScreenshotLabel();
        }

    }
}
