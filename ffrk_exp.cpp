#include "ffrk_exp.h"
#include "ui_ffrk_exp.h"

ffrk_exp::ffrk_exp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ffrk_exp),
    _tyro_exp(false),
    _current_level(-1),
    _desired_level(-1),
    _current_exp(0),
    _party_size(1),
    _run_exp(0),
    _absolute_exp(-1)
{
    ui->setupUi(this) ;

    read_data() ;
    QIntValidator *exp_valid = new QIntValidator(0, 20000000) ;
    ui->exp_run->setValidator( exp_valid ) ;
    ui->exp_input->setValidator( exp_valid ) ;
    QIntValidator *lvl_valid = new QIntValidator(1, _tyro.size()) ;
    ui->level_input->setValidator( lvl_valid ) ;
    ui->level_output->setValidator( lvl_valid ) ;
    ui->party_size->setValidator( new QIntValidator(1,5) ) ;
}

ffrk_exp::~ffrk_exp()
{
    delete ui;
}

/**
 * Reads in the experience points table data
 */
void ffrk_exp::read_data()
{
    QFile fi_tyro("exp_table_tyro") ;
    fi_tyro.open( QFile::ReadOnly ) ;
    if( fi_tyro.exists() ) {
        int lvl = 1 ;
        QTextStream tyro_exp( &fi_tyro ) ;
        while( !tyro_exp.atEnd() ) {
            QString exp = tyro_exp.readLine() ;
            std::pair<map_type::iterator, bool> result =
                    _tyro.insert( std::make_pair(lvl++, exp.toInt()) ) ;
            if( !result.second ) {
                /** Failed to insert into map **/
            }
        }
    } else {
        QMessageBox msg ;
        msg.setWindowTitle("Error opening Tyro Data file") ;
        msg.setText( fi_tyro.errorString() ) ;
        msg.exec() ;
    }

    QFile fi_other("exp_table") ;
    fi_other.open( QFile::ReadOnly ) ;
    if( fi_other.exists() ) {
        int lvl = 1 ;
        QTextStream general_exp( &fi_other ) ;
        while( !general_exp.atEnd() ) {
            QString exp = general_exp.readLine() ;
            std::pair<map_type::iterator, bool> result =
                    _general.insert( std::make_pair(lvl++, exp.toInt()) ) ;
            if( !result.second ) {
                /** Failed to insert into map **/
            }
        }
    } else {
        QMessageBox msg ;
        msg.setWindowTitle("Error opening Tyro Data file") ;
        msg.setText( fi_other.errorString() ) ;
        msg.exec() ;
    }
}

/**
 * Updates the exp value needed
 */
void ffrk_exp::update_exp_needed()
{
    QString tmp("Exp needed: ") ;
    if( _current_level < 0 || _desired_level < 0 ) {
        return ;
    } else
    if( _current_level <= _desired_level ) {
        map_type *curr = &_general ;
        if( _tyro_exp ) {
            curr = &_tyro ;
        }
        map_type::iterator It = curr->find( _current_level ) ;
        map_type::iterator It_end = curr->find( _desired_level ) ;
        int exp = _current_exp ;
        if( 0 < exp ) {
            It++ ;
        }
        while( true ) {
            exp += It->second ;
            if( It == It_end ) break ;
            It++ ;
        }
        _absolute_exp = exp ;
        tmp += QLocale(QLocale::English).toString(double(exp), 'f', 0) ;
    } else {
        tmp += "N/A" ;
    }
    ui->exp_output->setText( tmp ) ;
    update_exp_runs() ;
}

/**
 * Chooses which table to select the experience points from
 * Checked:   Tyro exp table
 * Unchecked: General exp table
 */
void ffrk_exp::on_tyro_check_stateChanged(int arg1)
{
    if( arg1 == Qt::Checked ) {
        _tyro_exp = true ;
    } else {
        _tyro_exp = false ;
    }
    update_exp_needed() ;
}

/**
 * Starting level, must be greater than 1
 */
void ffrk_exp::on_level_input_textChanged(const QString &arg1)
{
    _current_level = std::max(1, arg1.toInt()) ;
    ui->level_input->setText( QString::number(_current_level) ) ;
    _current_level++ ;
    update_exp_needed() ;
}

/**
 * Desired level, must be less than max number of levels
 */
void ffrk_exp::on_level_output_textChanged(const QString &arg1)
{
    _desired_level = std::min(int(_tyro.size()), arg1.toInt()) ;
    ui->level_output->setText( QString::number(_desired_level) ) ;
    update_exp_needed() ;
}

/**
 * Current Experience points value
 */
void ffrk_exp::on_exp_input_textChanged(const QString &arg1)
{
    _current_exp = arg1.toInt() ;
    update_exp_needed() ;
}

/**
 * Updates the estimate number of runs to complete the requested level
 */
void ffrk_exp::update_exp_runs()
{
    if( _absolute_exp < 0 || _party_size < 1 || _run_exp < 1 ) {
        return ;
    }
    double runs = _absolute_exp * double(_party_size) / double(_run_exp) ;
    QString tmp("Number of runs: ") ;
    tmp += QString::number(runs) ;
    ui->number_runs->setText( tmp ) ;
}

/**
 * Sets the current party size, divides the exp per run
 */
void ffrk_exp::on_party_size_textChanged(const QString &arg1)
{
    _party_size = arg1.toInt() ;
    update_exp_runs() ;
}

/**
 * Sets the current exp per run
 */
void ffrk_exp::on_exp_run_textChanged(const QString &arg1)
{
    _run_exp = arg1.toInt() ;
    update_exp_runs() ;
}
