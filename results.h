#ifndef RESULTS_H
#define RESULTS_H
#include "scheduler.h"
#include "oknotablitsa.h"
#include <QDialog>
#include <QTextEdit>
namespace Ui {
class results;
}

class results : public QDialog
{
    Q_OBJECT

public:
    explicit results(QWidget *parent = nullptr);
    ~results();
void showresults (const char* message, QList <scheduler::traffic*> error_traffics_list, QList <const _node*> error_nodes,QList <const _switch*> error_switches);
private:
    Ui::results *ui;
};

#endif // RESULTS_H
