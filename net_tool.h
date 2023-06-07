#ifndef NET_TOOL_H
#define NET_TOOL_H



#include <QMainWindow>
#include <qdir.h>
#include <QMessageBox>
#include <vector>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionValidator>
#include <QByteArray>
#include <QTabWidget>
#include <cmath>

//#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class net_tool; class Calc_fun;}
QT_END_NAMESPACE

class net_tool : public QMainWindow
{
    Q_OBJECT

public:
    net_tool(QWidget *parent = nullptr);
    ~net_tool();
//----------mac_calc----------------
    QString result;
    //读取输入值并保存
    QString style_result;
    bool Vendor_id_tag = false;
    //确认是否勾上输出供应商功能
    bool Vendor_read = false;
    //记录是否已读取供应商信息
    //QList<QVector<QString>> Mac_Vendor;
    QVector<QVector<QString>> mac_vendor;
    //定义MAC地址供应商查询结果
    QVector<QString> result_vendor;
    //定义MAC格式转换结果
    //定义全局变量以保证所有函数都能修改
    QByteArray byteTmp;//定义流，用于QString转换
    QStringList content_List;
    char *content_char;//定义字符组，用于读取QString
    int spli_num = 2;
    QString separator = "";
//----------mac_calc----------------

//---------ip_source----------------

    QString read_ip = "";
    QStringList read_ip_list;

    char *ip_format;
    unsigned int tmp;
    QStringList str_ip;
    QVector<unsigned int> int_Ip;
    QVector<QString> country;
    QVector<QString> province;
    QVector<QString> city;
    QVector<QString> isp;
    QString ip_final_result;
    QVector<QVector<QString>> ip_merge;
    bool ip_merge_is_read = false;
    QString tmp_read_ip= "";
    //QHostAddress test;
    QVector<unsigned int> ip_head;


//---------ip_source----------------

//---------base_covertion-----------

    QString base2_txt= "";
    QString base8_txt= "";
    QString base10_txt= "";
    QString base16_txt = "";
    unsigned long long  tmp_base10 = 0;





//---------base_covertion-----------

//-------------net_calc--------------
    QString net_txt = "";
    QStringList net_txt_list;
    QString net_calc_output_txt = "";





signals:

public slots:

    void print_Upper();
    void print_Lower();
    void mode1();
    void mode2();
    void mode3();
    void mode4();

//----------ip_source--------------



private slots:
//----------mac_calc-----------------------
    void on_clean_clicked();
    void on_Vendor_id_stateChanged();
    void read_mac_vendor();
    void calc_mac_vendor();
    void printResult(QString mode_result);
    void To_upper();
    void To_lower();
    void on_txtitpt_textChanged();
    void mac_format(int spli_num, QString separator);

//----------ip_source-----------------------
    void on_ip_Input_textChanged();
    void ipToint();
    void intToip();
    //bool conDB();
    void ip_source_Search();
    void read_ip_merge();
    void on_ip_Search_clicked();
    void on_clear_ip_source_clicked();
    void on_clear_ip_memory_clicked();
    void on_IPtoint_clicked();
    void on_inttoIP_clicked();

    int lower_bound(QVector<QVector<QString>> ip_merge , unsigned int target);


//-----------base_convertion----------------
    QString conver_1022(QString val);
    QString conver_1028(QString val);
    QString conver_10216(QString val);

    unsigned long long conver_2210(QString val);
    unsigned long long conver_8210(QString val);
    unsigned long long conver_16210(QString val);
    unsigned long long conver_check(unsigned long long val);

    void on_conver22_textEdited(const QString &arg1);

    void on_conver28_textEdited(const QString &arg1);

    void on_conver210_textEdited(const QString &arg1);

    void on_conver216_textEdited(const QString &arg1);


//-----------net_calc----------------




    void net_check(QStringList &net_txt_list);

    void on_net_input_textChanged();
    void on_net_calc_clicked();

    void on_net_calc_clear_clicked();

private:
    Ui::net_tool *ui;
};

class Calc_fun : public QMainWindow
{

};


//class Ip_source : public QMainWindow
//{
//    Q_OBJECT

//public:
//    Ip_source(QWidget *parent = nullptr);
//    ~Ip_source();
//    QString read_ip = "";
//    QStringList read_ip_list;
//    QString tmp_read_ip= "";
//    //QList<QVector<QString>> Mac_Vendor;
//    //QVector<QString> result_vendor;
//    //定义MAC格式转换结果
//    //定义全局变量以保证所有函数都能修改
//    QByteArray byteTmp;//定义流，用于QString转换
//    char *ip_format;


//signals:

//public slots:





//private slots:
//    void on_ip_Input_textChanged();

//private:
//    Ui::net_tool *ui;
//};





#endif // NET_TOOL_H
