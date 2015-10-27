#include "ffrk_exp.h"
#include "ui_ffrk_exp.h"

ffrk_exp::ffrk_exp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ffrk_exp),
    _tyro_exp(false),
    _current_level(-1),
    _desired_level(-1),
    _current_exp(0)
{
    ui->setupUi(this) ;

    read_data() ;
    QIntValidator *exp_valid = new QIntValidator(0, 20000000) ;
    ui->exp_input->setValidator( exp_valid ) ;
    QIntValidator *lvl_valid = new QIntValidator(1, _tyro.size()) ;
    ui->level_input->setValidator( lvl_valid ) ;
    ui->level_output->setValidator( lvl_valid ) ;
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
        QTextStream tyro_exp( &fi_tyro ) ;
        while( !tyro_exp.atEnd() ) {
            QStringList line = tyro_exp.readLine().split(",") ;
            std::pair<map_type::iterator, bool> result =
                    _tyro.insert( std::make_pair(line[0].toInt(), line[1].toInt()) ) ;
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
        QTextStream general_exp( &fi_other ) ;
        while( !general_exp.atEnd() ) {
            QStringList line = general_exp.readLine().split(",") ;
            std::pair<map_type::iterator, bool> result =
                    _general.insert( std::make_pair(line[0].toInt(), line[1].toInt()) ) ;
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
        int exp = 0 ;
        while( It != It_end ) {
            exp += It->second ;
            It++ ;
        }
        exp -= _current_exp ;
        tmp += QString::number( exp ) ;
    } else {
        tmp += "N/A" ;
    }
    ui->exp_output->setText( tmp ) ;
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
}

/**
 * Starting level, must be greater than 1
 */
void ffrk_exp::on_level_input_textChanged(const QString &arg1)
{
    _current_level = std::max(1, arg1.toInt()) ;
    ui->level_input->setText( QString::number(_current_level) ) ;
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
