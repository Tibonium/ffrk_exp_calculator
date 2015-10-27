#ifndef FFRK_EXP_H
#define FFRK_EXP_H

#include <QWidget>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>

#include <map>
#include <algorithm>

namespace Ui {
class ffrk_exp;
}

class ffrk_exp : public QWidget
{
    Q_OBJECT

public:
    explicit ffrk_exp(QWidget *parent = 0);
    ~ffrk_exp();

private slots:
    void on_tyro_check_stateChanged(int arg1);

    void on_level_input_textChanged(const QString &arg1);

    void on_level_output_textChanged(const QString &arg1);

    void on_exp_input_textChanged(const QString &arg1);

    void on_party_size_textChanged(const QString &arg1);

    void on_exp_run_textChanged(const QString &arg1);

private:

    typedef std::map<int,int>       map_type ;
    map_type _tyro ;
    map_type _general ;

    Ui::ffrk_exp *ui;

    void read_data() ;
    bool _tyro_exp ;
    int _current_level ;
    int _desired_level ;
    int _current_exp ;
    void update_exp_needed() ;
    int _party_size ;
    int _run_exp ;
    int _absolute_exp ;
    void update_exp_runs() ;
};

#endif // FFRK_EXP_H
