#ifndef OKNOTABLITSA_H
#define OKNOTABLITSA_H
#include <QTableWidgetItem>
#include <QDialog>
#include <QLineEdit>
#include <QDebug>
#include "xmlparser.h"
#include "scheduler.h"
#define MAX_COL 256
#define MAX_ROW 100
using namespace std;
using namespace scheduler_ns;
namespace Ui {
class oknotablitsa;
}

class oknotablitsa : public QDialog
{
    Q_OBJECT

public:
    explicit oknotablitsa(char *argv[],QWidget *parent = nullptr);
    ~oknotablitsa();
    uint mas [MAX_ROW][MAX_COL];
    void fillpos();
   _topologyAndNetwork topology;
    scheduler::epoch epochdata; //храним все что связано с эпохой
   uint epochlenght;//max длина эпохи в таблице
   uint step; //шаг между врем. интервалами,мс
   uint zagruzka; //max загрузка каналов %
   uint packetsize; //запас размера пакета в байтах
   uint tochno;//точность вычислений
   uint numbercolumn;//в построении таблицы
   QList <scheduler::scheduling_table_string> scheduling_table;
   QList <dl> time_slots_start; //ВРЕМЯ НАЧАЛА ТАЙМ-СЛОТОВ
   QList <scheduler::traffic> traffics_list; //СПИСОК ТРАФИКОВ
   QMap <QList <ull>, QList <scheduler::traffic_table_el>> conflict_traffics_table;//конфликтные трафики
   QList <scheduler::group> traffic_groups;//группы трафиков
   QList <scheduler::traffic*> error_traffics_list;//списоб некорректных трафиков
   QList <scheduler::node> nodes_list;//список узлов всей сети
   QList <QList <scheduler::traffic*>> error_conflicts_list;//список некорректных конфликтов
   QList <scheduler::group*> scheduling_groups;//группы планируемых трафиков
   QList <QList <scheduler::traffic*>> conflicts_list;//список корректных конфликтов
   QList <const _node*> error_nodes;//список некорректных узлов
   QList <const _switch*> error_switches;//список некорректных коммутаторов
   QList <scheduler::checktable> checktable_;
   QList <scheduler::TGEN> tgenl;
   scheduler sch;

protected slots:
 void on_pushButton_2_clicked();
  void on_pushButton_4_clicked();
   void on_pushButton_5_clicked();
   void on_pushButton_7_clicked();
   void on_pushButton_3_clicked();
    void on_pushButton_8_clicked();
private slots:





   void on_pushButton_6_clicked();





private:
    uint numberrow_;
    Ui::oknotablitsa *ui;
};

#endif // OKNOTABLITSA_H
