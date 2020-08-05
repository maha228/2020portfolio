#include "oknotablitsa.h"
#include "ui_oknotablitsa.h"
#include "results.h"
#include "ui_rezults.h"
#include <QList>
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QLine>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

using namespace scheduler_ns;
oknotablitsa::oknotablitsa(char *argv[],QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oknotablitsa)
{

    ui->setupUi(this);
    setWindowTitle("Проверка корректности таблицы расписания СТП-ИСС-14");
    setFixedSize(1200,700); //размер окна
    //_topologyAndNetwork topology;
        XmlParser xmlParser;
        int parserExitCode = 0;
        parserExitCode = xmlParser.doParsingTopology(argv, topology);
        if (parserExitCode == 4)
        {
        cout <<"incorrect input file" <<endl;
        }
        else
        { cout <<"correct input file" <<endl;
    }  
        numberrow_=sch.get_sched_nodes_num(topology);

}


oknotablitsa::~oknotablitsa()
{
    delete ui;
}

void oknotablitsa::on_pushButton_2_clicked() //ЗАКРАСКА ЯЧЕЙКИ
{
    QTableWidgetItem *tbl = ui->tableWidget->currentItem();
    tbl->setForeground(Qt::darkGreen);
    tbl->setText(QString::number(1));
    tbl->setBackground(Qt::darkGreen); //при выделении цветов запись единицы в ячейку такого же цвета
}



void oknotablitsa::on_pushButton_4_clicked() //ОЧИСТИТЬ ТАБЛИЦУ
{
    int n=0;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
           for( int j=0; j<ui->tableWidget->columnCount();j++){
               QTableWidgetItem *nol= new QTableWidgetItem(QString::number(n));
               nol->setForeground(Qt::white);
    ui->tableWidget->setItem(i,j,nol); //заполнить нулями
}
    }
}

void oknotablitsa::on_pushButton_5_clicked()//КНОПКА "ПРОВЕРИТЬ"
{

    dl runtime = 0;
    const char *error_message = "";
uint nodes_num;
nodes_num = sch.get_sched_nodes_num(topology);


 //  cout<<epochlenght<<"epoch,ms"<<endl;

   fillpos();
   for(int i=0;i<scheduling_table.size();i++){
       cout<<"string "<<scheduling_table.at(i).string_id<<endl;
       for (int j=0;j<scheduling_table.at(i).positions_list.size();j++) {
           cout<<"["<<scheduling_table.at(i).positions_list.at(j).first_time_slot<<";"
              <<scheduling_table.at(i).positions_list.at(j).last_time_slot<<"]";
       }
       cout<<endl;
   }
   sch.fil_parameters(epochdata,zagruzka,packetsize,tochno);
   cout<<"vse klevo"<<endl;
   bool primary_checks_result =true;
   bool minimal_delay_result =true;
   bool epoch_result=true;
   bool capacity_result=true;
   bool necessty_result=true;
   bool scheduling_result=true;
   bool iscorrect= true;
   QList <dl> runtime_mas;
   runtime_mas.clear();
   bool time_fail=true;
   traffics_list.clear();
   conflict_traffics_table.clear();
   traffic_groups.clear();
   error_message="";
   error_traffics_list.clear();
   error_nodes.clear();
   error_switches.clear();
   runtime=0;
   nodes_list.clear();
   error_conflicts_list.clear();
   scheduling_groups.clear();
   conflicts_list.clear();
   checktable_.clear();
      if (iscorrect==true){
          traffics_list.clear();
       /*ПРОВЕРКА ШАГ 1*/ //проверка корректности топологии (все ли в сети на месте)
        primary_checks_result = sch.primary_checks(topology, traffics_list, conflict_traffics_table, traffic_groups, error_message, error_traffics_list, error_nodes, error_switches, runtime);
                   if (!primary_checks_result) {

                       /*is_err = true;*/cout<<"step 1 incorrect"<<endl;
                   iscorrect=false;
                   }
                   else{
                       cout<<"step 1 correct"<<endl;
                       iscorrect=true;
                   }
   }

   if(iscorrect==true){
        /* error_traffics_list.clear();
         error_message= "";
         nodes_list.clear();
          minimal_delay_result=true;
          runtime=0;*/
       /*ПРОВЕРКА ШАГ 2*/ //проверка минимальных задержек на передачу
       minimal_delay_result = sch.not_acceptable_traffics(topology, traffic_groups, traffics_list, nodes_list, error_message, error_traffics_list, runtime);
       if (!minimal_delay_result) {

           /*is_err = true;*/cout<<"step 2 incorrect"<<endl;
       iscorrect=false;
       }
       else{
           cout<<"step 2 correct"<<endl;
           iscorrect=true;
       }
   }
   if(iscorrect==true){
        // error_traffics_list.clear();
       /*ПРОВЕРКА ШАГ 3*/ //проверка связанная с эпохой
       epoch_result = sch.get_epoch_duraton(traffic_groups, error_message, error_traffics_list, runtime, epochdata.time_slot_num,epochdata);
       if (!epoch_result) {

           /*is_err = true;*/cout<<"step 3 incorrect"<<endl;
       iscorrect=false;
       }
       else{
           cout<<"step 3 correct"<<endl;
           iscorrect=true;
       }
   }
   epochdata.epoch_lcm.convert_unit(scheduler::ms);
   uint lcm= epochdata.epoch_lcm.value;
   cout<<epochdata.max_epoch_duration.value<<"     max"<<endl;
   cout<<epochdata.epoch_lcm.value<<"     LCM"<<endl;
   ui->lineEdit_4->setText(QString::number(lcm));
   if(iscorrect==true){
        // error_traffics_list.clear();
       /*ПРОВЕРКА ШАГ 4*/ //проверка связанная с пропускной спосоностью каналов в сети
       capacity_result = sch.check_capacity(conflict_traffics_table, error_message, error_traffics_list, error_nodes, error_switches, runtime);
       if (!capacity_result) {

           /*is_err = true;*/cout<<"step 4 incorrect"<<endl;
       iscorrect=false;
       }
       else{
           cout<<"step 4 correct"<<endl;
           iscorrect=true;
       }
   }

       if(iscorrect==true){
           /*ПРОВЕРКА ШАГ 5*/ //проверка нужна ли таблица для данной сети
           error_traffics_list.clear();
            conflicts_list.clear();
           necessty_result = sch.check_scheduling_necessity(traffics_list, conflict_traffics_table, error_message, traffic_groups, conflicts_list,
                           error_traffics_list, error_conflicts_list, scheduling_groups, runtime);
           cout<<"conflicts "<<conflicts_list.size()<<endl;

           if (!necessty_result) {

               /*is_err = true;*/cout<<"step 5 incorrect"<<endl;
           iscorrect=false;
           }
           else{
               cout<<"step 5 correct"<<endl;
               iscorrect=true;
           }
       }
      if(iscorrect==true){
       uint strid=0;
       for(uint i=0;i<nodes_list.size();i++){
           if ((nodes_list.at(i).node_p->node_unit_list.at(0).transport_layer_list.at(0).name == "stp_iss_14") &&
           (nodes_list.at(i).node_p->node_unit_list.at(0).transport_layer_list.at(0).stp_iss_setting.m_scheduling_qos)){
               scheduler::checktable ch;
               ch.node_p= &nodes_list.at(i);
               ch.string_p= &scheduling_table.at(strid);
               strid++;
               checktable_.append(ch);
           }

       }

       }
       if(iscorrect==true){
           /*ПРОВЕРКА ШАГ 6*/ //правильно ли построено расписание
           //  error_traffics_list.clear();
          scheduling_result = sch.scheduling(traffics_list, nodes_list, conflicts_list, error_message, scheduling_groups, error_traffics_list, runtime_mas, time_fail,checktable_,tgenl);
          if (!scheduling_result) {

              cout<<"step 6 incorrect"<<endl;
          iscorrect=false;
          }
          else{
              cout<<"step 6 correct"<<endl;
              iscorrect=true;
          }
       }


if(iscorrect==false){

      /* char error_message_[2000];
       char dop1[1000];
       sprintf(error_message_, "  дополнение 0");
       sprintf(dop1, "  дополнение 1");
       strcat(error_message_, dop1);
       sprintf(dop1, "  дополнение 2");
       strcat(error_message_, dop1);
       QMessageBox::critical(this,"ERROR",error_message);*/
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Ошибка","Таблица расписания построена некорректно. Просмотреть результаты проверки?", QMessageBox::Yes | QMessageBox::No);
           if(reply == QMessageBox::Yes){
               results window;
               window.setModal(true);
               window.showresults(error_message,error_traffics_list,error_nodes,error_switches);
               window.exec();

           }
           else{
    //cout<<"biba"<<endl;
           }
}
else{
    QMessageBox::about(this,"correct","Заданная таблица расписания корректна");
}



}

void oknotablitsa::on_pushButton_7_clicked() //СОХРАНЕНИЕ ПАРАМЕТРОВ
{

    epochlenght=0;
    step=0;
    zagruzka=0;
    packetsize=0;
    tochno=1;
    tgenl.clear();
    if(ui->lineEdit->text().isEmpty() || /*ui->lineEdit_4->text().isEmpty() ||*/ ui->lineEdit_3->text().isEmpty() || ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty())
    {
     QMessageBox::warning(this,"предупреждение","введите все параметры");
    }
    else if (ui->lineEdit_3->text().toUInt()>1024 ||ui->lineEdit_3->text().toUInt()<=0 ) {
        QMessageBox::warning(this,"предупреждение","Длина эпохи должна лежать в диапазоне от 0 мс до 1024 мс");

    }
    else if (ui->lineEdit_5->text().toUInt()>1) {

       QMessageBox::warning(this,"предупреждение","шаг между временными интервалами может быть от 0 до 1");
    }
    else if (ui->lineEdit_6->text().toUInt()==0 ||ui->lineEdit_6->text().toUInt()>100 ) {
        QMessageBox::warning(this,"предупреждение","загрузка каналов возможна от 1% до 100%");
    }
   /* else if (ui->lineEdit_4->text().toUInt()==0 ||ui->lineEdit_4->text().toUInt()>35) {
        QMessageBox::warning(this,"предупреждение","точность вычислений должна быть в интервале от 1 до 35");
    }*/
    else{


        epochlenght=ui->lineEdit_3->text().toUInt();//запись параметра максимальная длина эпохи в переменную
        step=ui->lineEdit_5->text().toUInt(); //шаг между врем. интервалами,мс
        zagruzka=ui->lineEdit_6->text().toUInt(); //max загрузка каналов %
        packetsize=ui->lineEdit->text().toUInt(); //запас размера пакета в байтах
       // tochno=ui->lineEdit_4->text().toUInt();//точность вычислений
        QList<unsigned long long> nodeId;
        QList < QList <unsigned long long>> trafId;
        uint tranum=0;
        for (int node_i = 0; node_i < topology.node_elements_list.size(); node_i++)
        {
        if ((topology.node_elements_list.at(node_i).node_unit_list.at(0).transport_layer_list.at(0).name == "stp_iss_14") &&
        (topology.node_elements_list.at(node_i).node_unit_list.at(0).transport_layer_list.at(0).stp_iss_setting.m_scheduling_qos)){

        nodeId.append(topology.node_elements_list.at(node_i).node_id);
        }
        QList <unsigned long long> tid;

        cout<<"trafics "<<topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.size()<<endl;
        for(uint r=0;r<topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.size();r++){
            tid.append(topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.at(r).identificationNumber);
            //bool tack=topology.node_elements_list.at(node_i).node_unit_list.at(0).transport_layer_list.at(0).stp_iss_setting.m_guaranteed_qos;

        }
        tranum+=topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.size();
        if(topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.size()>0){
             trafId.append(tid);
        }
        }
        if(tranum<2){
            QMessageBox::warning(this,"предупреждение","В сети задано менее 2 трафиков с включенным качеством сервиса 'планирование'");
        }
        epochdata.epoch_lcm.value=0;
        epochdata.epoch_lcm.unit=scheduler::us;
              numbercolumn= ui->lineEdit_2->text().toUInt();
            scheduling_table.clear();
            epochdata.time_slot_num=0;
            epochdata.max_epoch_duration.value=0;
            epochdata.time_slot_duration.value = 0;
            epochdata.time_slot_duration.unit = scheduler::us;
            epochdata.delta_time_slot.value=0;
            epochdata.delta_time_slot.unit=scheduler::us;
            time_slots_start.clear();
            epochdata.time_slot_num=numbercolumn;
            epochdata.max_epoch_duration.value=epochlenght;
            epochdata.max_epoch_duration.unit=scheduler::ms;
            epochdata.max_epoch_duration.convert_unit(scheduler::us);
         /* Вычисление длительности тайм-слота */
            epochdata.time_slot_duration.value = epochdata.max_epoch_duration.value / epochdata.time_slot_num;
            epochdata.time_slot_duration.unit = scheduler::us;
            epochdata.delta_time_slot.value=step;
            epochdata.delta_time_slot.unit=scheduler::ms;
            epochdata.delta_time_slot.convert_unit(scheduler::us);

            /* Начало каждого тайм-слота записыватся в time_slots_start */
                for (ull ts = 0; ts <= epochdata.time_slot_num; ts++)
                {
                    time_slots_start.append(ts * (epochdata.time_slot_duration.value + epochdata.delta_time_slot.value));
                    cout<<"tt"<<time_slots_start.at(ts)<<endl;
                }
                cout<<epochdata.time_slot_duration.value<<"duration"<<endl;
                cout<<epochdata.delta_time_slot.value<<"delta"<<endl;

                 if (numberrow_>10) {
                    QMessageBox::warning(this,"предупреждение","количество узлов не должно превышать 10");
                    return;

                 }
                 if (numberrow_<2){
                    QMessageBox::warning(this,"предупреждение","Минимальное число узлов - 2");
                    return;
                 }
                 if (numbercolumn>256 || numbercolumn<2){
                    QMessageBox::warning(this,"предупреждение","Количество тайм-слотов должно быть в диапазоне от 2 до 256");
                    return;
                 }

                /* if (numbercolumn<2){
                    QMessageBox::warning(this,"предупреждение","Минимальное число тайм-слотов - 2");
                    return;
                 }*/
                 else {
                     ui->tableWidget->setRowCount(numberrow_+1);// количество строк в таблице
                     ui->tableWidget->setColumnCount(numbercolumn);//количество столбцов в таблице



                     for(int s=0;s<ui->tableWidget->rowCount()-1;s++)
                       for(int c=0;c<ui->tableWidget->columnCount();c++){
                           QTableWidgetItem *it = new QTableWidgetItem(QString::number(0));
                           it->setForeground(Qt::white);
                             ui->tableWidget->setItem(s,c,it);//заполняем таблицу белыми нулями для записи и проверки
                       }
                    }

                 for(int c=0;c<ui->tableWidget->columnCount();c++){
                     int number=(int)(time_slots_start.at(c)/1000);
                    // cout<<time_slots_start.at(c)<<")))))"<<endl;
                     cout<<number<<"iii"<<endl;
                 QTableWidgetItem *it = new QTableWidgetItem(QString::number(number)+"ms");
                 it->setForeground(Qt::darkRed);
                   ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,c,it);//заполняем таблицу ДЛЯ ВРЕМЕНИ ПОСЛЕДНЕЙ СТРОКИ
             }

                 QStringList lst;
                  QStringList lsb;
                 for(uint i=1;i<=numberrow_;i++){

                     QString s =QString::number(nodeId.at(i-1)) + " node( " ;
                     for (uint f=0;f<trafId.at(i-1).size();f++) {
                         if(f>0){
                             s+=", ";
                         }
                         s+=QString::number(trafId.at(i-1).at(f));

                     }
                     s+=" )";
                     lst << s;
                     ui->tableWidget->setVerticalHeaderLabels(lst); //именуем строки
                     //ui->tableWidget_2->setVerticalHeaderLabels(lst); //именуем строки

                 }
                 QString s = "начало, мкс" ;
                 lst << s;
                 ui->tableWidget->setVerticalHeaderLabels(lst); //именуем строки
                 // ui->tableWidget_2->setVerticalHeaderLabels(lst); //именуем строки
                 for(uint j=0;j<=numbercolumn;j++){
                     QString k =QString::number(j) + "-slot" ;
                     lsb << k;
                     ui->tableWidget->setHorizontalHeaderLabels(lsb); //именуем столбцы
                    // ui->tableWidget_2->setHorizontalHeaderLabels(lsb); //именуем столбцы


                 }
                cout<<"4h "<<topology.node_elements_list.at(0).node_id<<endl;
        }
        cout<<"1 "<<epochlenght<<" 2 "<<step<<" 3 "<<zagruzka<<" 4 "<<packetsize<<" 5 "<<tochno<<endl;
    }

void oknotablitsa::fillpos(){//ФУНКЦИЯ записываем из массива таблицу в нормальную структуру
    scheduler::position pos;
    scheduler::scheduling_table_string str;//строка позиций
    bool posend=false; //конец позиции
    for (int f=0;f<ui->tableWidget->rowCount();f++) {
        str.string_id=f;//номер строки
        posend=false;
        QList<scheduler::position> poslist; //лист в который мы будем пихать  позиций
        for (int g=0;g<=ui->tableWidget->columnCount();g++) {
            if(posend==false&&mas[f][g]==1){ //начало зелени в таблице
                pos.first_time_slot=g;
                posend=true;
                cout<<"poz["<<pos.first_time_slot;

            }
            else if (posend==true&&mas[f][g]==0) { //конец зелени в таблице
                pos.last_time_slot=g-1;
                posend=false;
                cout<<"; "<<pos.last_time_slot<<"]"<<endl;
                poslist.append(pos);//все позиции в строке
            }
        }
        str.positions_list= poslist;
        scheduling_table.append(str);

        }

}

void oknotablitsa::on_pushButton_6_clicked() //ОТМЕНЯЕМ ЗАКРАСКУ
{
    QTableWidgetItem *tbl = ui->tableWidget->currentItem();
    tbl->setForeground(Qt::white);
    tbl->setText(QString::number(0));
    tbl->setBackground(Qt::white); //при выделении цветов запись единицы в ячейку такого же цвета
}

void oknotablitsa::on_pushButton_3_clicked() //ЗАДАЕМ ЗАДЕРЖКУ
{

     uint column;
     uint ID;
     ID=ui->lineEdit_7->text().toUInt();
     column=ui->lineEdit_8->text().toUInt();
     uint yzel;
     uint ni=0;
    QString eshe;
     for (int node_i = 0; node_i < topology.node_elements_list.size(); node_i++)
     {
         if ((topology.node_elements_list.at(node_i).node_unit_list.at(0).transport_layer_list.at(0).name == "stp_iss_14") &&
         (topology.node_elements_list.at(node_i).node_unit_list.at(0).transport_layer_list.at(0).stp_iss_setting.m_scheduling_qos)){

             for(uint r=0;r<topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.size();r++){
                if(topology.node_elements_list.at(node_i).node_unit_list.at(0).aplication_layer_list.at(0).messages.stp_messages.at(r).identificationNumber==ID){
                  yzel = ni ;

                        QTableWidgetItem *it = new QTableWidgetItem(QString::number(ID));
                        QBrush kistochka= ui->tableWidget->item(yzel,column)->background();
                        QBrush fintochka=ui->tableWidget->item(yzel,column)->foreground();
                        if(kistochka==Qt::darkGreen){
                            cout<<"was green "<<endl;
                        }
                       if(kistochka==Qt::darkGreen){
                           it->setForeground(Qt::white);
                           it->setBackground(kistochka);
                       }
                       else {
                           it->setForeground(Qt::darkMagenta);
                           it->setBackground(kistochka);
                       }

                       if(ui->tableWidget->item(yzel,column)->text().toUInt()==1 || ui->tableWidget->item(yzel,column)->text().toUInt()==0){
                        ui->tableWidget->setItem(yzel,column,it);
                       }
                       else{
                           eshe=ui->tableWidget->item(yzel,column)->text();
                           QTableWidgetItem *itit = new QTableWidgetItem(eshe+" "+QString::number(ID));
                           if(kistochka==Qt::darkGreen){
                               itit->setForeground(Qt::white);
                               itit->setBackground(kistochka);
                           }
                           else {
                               itit->setForeground(Qt::darkMagenta);
                               itit->setBackground(kistochka);
                           }
                        ui->tableWidget->setItem(yzel,column,itit);
                       }
                }
             }
        ni++;
         }
     }
     cout<<"row "<<yzel<<endl;
     cout<<"column "<<column<<"  ID TRAF  "<<ID<<endl;
     bool find= false;
     for(uint n=0;n<tgenl.size();n++){
         if(tgenl.at(n).trafid==ID){
             tgenl[n].timeslot =column;
             cout<<"tgcolumn  "<<tgenl.at(n).timeslot<<"  tgtrid  "<<tgenl.at(n).trafid<<endl;
             find= true;
         }

}
     if(find==false){
         scheduler::TGEN tg;
         tg.trafid=ID;
         tg.timeslot=column;
         tgenl.append(tg);
         cout<<"else tgcolumn  "<<tgenl.last().timeslot<<"  tgtrid  "<<tgenl.last().trafid<<endl;
     }
}

void oknotablitsa::on_pushButton_8_clicked()//СОХРАНИТЬ ТАБЛИЦУ
{
    for(int c=0;c<100;c++){
        for(int v=0;v<100;v++){
            mas[c][v]=0;
       }
    }
   for(int i=0;i<ui->tableWidget->rowCount();i++){
          for( int j=0; j<ui->tableWidget->columnCount();j++){
               QString poz;
               QString zop;
             poz=ui->tableWidget->item(i,j)->text();
             zop=poz.at(0);
              mas [i][j]=zop.toUInt(); //запись таблицы в массив
               }
   }
bool masempty = true;
   for (int f=0;f<ui->tableWidget->rowCount();f++) {
       for (int g=0;g<ui->tableWidget->columnCount();g++) {
           if(mas[f][g]==1){
               masempty=false;

           }
          std::cout<< mas[f][g]; //одна строка таблицы
       }
   std::cout<<std::endl; //переход новая строка
   }
   if(masempty==true){
       QMessageBox::warning(this,"ошибка","Таблица расписания не заполнена");
       return;
   }
   for (int f=0;f<ui->tableWidget->rowCount()-1;f++) {
       bool stringempty=true;
       for (int g=0;g<ui->tableWidget->columnCount();g++) {
       if(mas[f][g]==1){
           stringempty=false;
       break;
       }

       }
       if(stringempty==true){
           QMessageBox::warning(this,"ошибка","Cтрока не заполнена");
           return;
       }
       }
}
