#include "dbconnect.h"

bool createConnection(QString host, QString database)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug() << QSqlDatabase::drivers();
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName("root");
    db.setPassword("root");
    bool ok = db.open();
    if(ok){
        foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            MainWindow *nw = qobject_cast<MainWindow*>(widget);
            if(nw) nw->setStatus("Connected");
        }
    }
    return ok;
}

int father_id(int gender, int id_child){

    QSqlQuery query;
    query.prepare("SELECT id_father FROM Relations WHERE id_child = :ID AND is_bull = :GENDER");
    query.bindValue(":ID", id_child);
    query.bindValue(":GENDER", gender);

    if(query.exec()){
        if(query.size()==0) return -2;
        query.first();
        int id = query.value(0).toInt();
        return id;
    }
    return -1;

}

int mother_id(int gender, int id_child){

    QSqlQuery query;
    query.prepare("SELECT id_mother FROM Relations WHERE id_child = :ID AND is_bull = :GENDER");
    query.bindValue(":ID", id_child);
    query.bindValue(":GENDER", gender);

    if(query.exec()){
        if(query.size()==0) return -2;
        query.first();
        int id = query.value(0).toInt();
        return id;
    }
    return -1;

}

QString father_name(int id){

    QSqlQuery query;
    QString name;
    query.prepare(QString("SELECT Name FROM bulls WHERE idBulls =%1").arg(id));
    //query.bindValue(":ID", id);


    if(query.exec()){
        if(query.size()==0) return "";
        query.first();
        name = query.value(0).toString();

        return name;
    }
    return query.lastError().text();
}

QString mother_name(int id){

    QSqlQuery query;
    QString name;
    query.prepare(QString("SELECT Name FROM cows WHERE idCows =%1").arg(id));
    //query.bindValue(":ID", id);


    if(query.exec()){
        if(query.size()==0) return "";
        query.first();
        name = query.value(0).toString();

        return name;
    }
    qDebug() << "error" << query.lastError().text();
    return query.lastError().text();
}

int father_number(int id){

    QSqlQuery query;
    int name;
    query.prepare(QString("SELECT Number FROM bulls WHERE idBulls =%1").arg(id));
    //query.bindValue(":ID", id);


    if(query.exec()){
        if(query.size()==0) return 0;
        query.first();
        name = query.value(0).toInt();

        return name;
    }
    return -1;
}

int mother_number(int id){

    QSqlQuery query;
    int name;
    query.prepare(QString("SELECT Number FROM cows WHERE idCows =%1").arg(id));
    //query.bindValue(":ID", id);


    if(query.exec()){
        if(query.size()==0) return 0;
        query.first();
        name = query.value(0).toInt();

        return name;
    }
    qDebug() << "error" << query.lastError().text();
    return -1;
}

