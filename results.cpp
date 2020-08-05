#include "results.h"
#include "ui_results.h"

results::results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::results)
{
    ui->setupUi(this);
setWindowTitle("Результаты проверки таблицы расписания СТП-ИСС-14");

}

results::~results()
{
    delete ui;
}
void results::showresults(const char *message, QList<scheduler::traffic *> error_traffics_list, QList<const _node *> error_nodes, QList<const _switch *> error_switches){

    ui->textEdit->setText("В ходе проверки таблицы расписания выявлена(-ы) следующая(-ие) ошибка(-и):");
    ui->textEdit->append(message);
    if(error_traffics_list.size()>0 && error_traffics_list.at(0)->source_node_p!=NULL){
         ui->textEdit->append("Рекомендуется внести изменения в следующий(-ие) трафик(-и):");
         for (uint i=0;i<error_traffics_list.size();i++) {
         // QString err;
          //err.append());
          ui->textEdit->append(QString::number(error_traffics_list.at(i)->traf_p->identificationNumber)+"(node "+ QString::number(error_traffics_list.at(i)->source_node_p->node_id) +")");
         }
    }
    if(error_nodes.size()>0){
        ui->textEdit->append("Рекомендуется внести изменения в следующий(-ие) узел(-ы):");
        for (uint j=0;j<error_nodes.size();j++) {
            ui->textEdit->append(QString::number(error_nodes.at(j)->node_id));
        }
    }
    if(error_switches.size()>0){
         ui->textEdit->append("Рекомендуется внести изменения в следующий(-ие) коммутатор(-ы):");
         for(uint z=0;z<error_switches.size();z++){
             ui->textEdit->append(QString::number(error_switches.at(z)->switch_id));
         }
    }
    //ui->textEdit->append(to_string(G).c_str());
}
