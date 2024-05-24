#ifndef MAC_CALC_H
#define MAC_CALC_H



#include <net_tool.h>
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


class Mac_calc : public net_tool
{
    Q_OBJECT

public:
    Ui::net_tool *ui;
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





signals:

private slots:
    void print_Upper()
    {
        To_upper();
        printResult(style_result);
    }
    void print_Lower()
    {
        To_lower();
        printResult(style_result);
    }
    void mode1()
    {
        style_result = result;
        printResult(style_result);
    }
    void mode2()
    {
        mac_format(4,".");
    }
    void mode3()
    {
        mac_format(2,":");
    }
    void mode4()
    {
        spli_num = ui->spli_num_box->value();
        separator = ui->spli_char_box->displayText();
        mac_format(spli_num,separator);
    }
    //----------mac_calc-----------------------
    void clean_clicked()
    {
        // 清除结果区域前面缓存的结果
        ui->txtotpt->clear();
        ui->txtitpt->clear();
        result = "";
    }
    void on_Vendor_id_stateChanged()
    {
        if (ui->Vendor_id->isChecked())
        {
            Vendor_id_tag = true;
            if (Vendor_read == false)
                read_mac_vendor();
            Vendor_read = true;
            //qDebug("yes,checked");
        }
        else
        {
            Vendor_id_tag = false;
            //qDebug("no!");
        }
    }
    void read_mac_vendor()
    {

        //QString path = "";
        //QDir currdir;
        mac_vendor.resize(31000);
        for (int k = 0; k < mac_vendor.size(); k++)
        {
            mac_vendor[k].resize(2);
        }
        //path = currdir.currentPath() + "/oui.csv";
        //QMessageBox::warning(0,"PATH",path,QMessageBox::Yes);
        //QFile readFile(path);

        QFile readFile(":/data/oui.csv");
        if (!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream *tmp_csv = new QTextStream(&readFile);
        //qDebug() << &tmp_csv;
        //qDebug() << tmp_csv;
        QStringList tmp_lines = tmp_csv->readAll().split("\n");
        //qDebug() << tmp_lines;

        //QMessageBox::warning(0,"test",tmp_lines[30000],QMessageBox::Yes);

        for (int i = 0; i < tmp_lines.count(); i++)
        {
            QString tmp_vendor = "";
            QStringList tmp = tmp_lines.at(i).split(",");
            //int tmp_compare = tmp.indexOf("\"");

            int ven_num = 0;
            for (int j = 0; j < tmp.count(); j++)
            {
                int tmp_compare = tmp[j].indexOf("\"");
                //qDebug() << tmp[j];
                if (tmp[j].startsWith("\""))
                    tmp_vendor = tmp_vendor + tmp[j] + ",";
                else if (tmp_compare == -1 && tmp_vendor != "")
                    tmp_vendor = tmp_vendor + tmp[j] + ",";

                else if (tmp[j].endsWith("\""))
                {
                    tmp_vendor = tmp_vendor + tmp[j];
                    tmp_vendor = tmp_vendor.replace("\"", "");
                    mac_vendor[i][ven_num] = tmp_vendor;
                    //qDebug() << "yeah!!!!"  << mac_vendor[i][ven_num];
                    ven_num++;
                    tmp_vendor = "";
                }
                else
                {
                    tmp_vendor = tmp[j];
                    //mac_vendor[i].append(tmp_vendor);
                    mac_vendor[i][ven_num] = tmp_vendor;
                    //qDebug() << mac_vendor[i][ven_num];
                    ven_num++;
                    //qDebug() << tmp_vendor;
                    tmp_vendor = "";
                }
            }
        }
        //qDebug() << mac_vendor[10844][1] << '\t' << mac_vendor[3086][2];
        readFile.close();
    }
    void calc_mac_vendor()
    {
        QString tmp = "";
        QTextStream *tmp_result = new QTextStream(&result);
        QStringList result_lines = tmp_result->readAll().split("\n");
        tmp_result = nullptr;
        //qDebug() << result_lines.count() << '\t' << mac_vendor.size();
        result_vendor.resize(result_lines.count());
        for (int i = 0; i < result_lines.count(); i++)
        {
            for (int j = 0; j < mac_vendor.size(); j++)
            {
                //if (mac_vendor[j][1] == "")
                //    break;
                //qDebug() << tmp << '\t' << mac_vendor[j][1];
                tmp = result_lines.at(i).toUpper();

                //qDebug() << tmp << '\t' << mac_vendor[j][1];

                if (tmp.indexOf(mac_vendor[j][0]) == 0)
                {
                    //qDebug() << tmp.indexOf(mac_vendor[j][1]);
                    result_vendor[i] = mac_vendor[j][1];
                    //qDebug() << result_vendor[i];
                    break;
                }
            }
        }
    }

    void printResult(QString mode_result)
    {
        ui->txtotpt->clear();
        if (Vendor_id_tag == false)
            ui->txtotpt->append(mode_result); // 如果没勾选输出MAC供应商，将结果输出到结果区域
        else
        {
            calc_mac_vendor();
            QString tmp_result = "";
            QTextStream *tmp_mode2 = new QTextStream(&mode_result);
            QStringList tmps = tmp_mode2->readAll().split("\n");
            for (int k = 0; k < tmps.count(); k++)
            {
                tmp_result = tmp_result + tmps.at(k) + '\t' + result_vendor[k]  ;
                if (k != tmps.count() - 1)
                {
                    tmp_result = tmp_result + '\n';
                }
                //qDebug() << result_vendor[k];
            }
            mode_result = tmp_result;
            ui->txtotpt->append(mode_result);
        }
    }

    void To_upper()
    {
        result = result.toUpper();
        style_result = style_result.toUpper();
    }
    void To_lower()
    {
        result = result.toLower();
        style_result = style_result.toLower();
    }
    void txtitpt_textChanged()
    {
        // 清除结果区域前面缓存的结果
        //ui->txtotpt->clear();
        result = "";
        //  获取输入框的内容
        QString content = ui->txtitpt->toPlainText(); //读取左边区域数据
        //ui->txtitpt->append(content);
        content_List = content.split('\n');
        for (int k = 0; k < content_List.size(); ++k) {
            byteTmp = content_List[k].toLatin1();
            char *content_char = byteTmp.data(); // 将QString转化为char字符数组
            for (int i = 0; i < content_List[k].size() ; i++) //获取输入的MAC并转为小写无字符到result
            {
                //qDebug() << content_char[i];

                if ((content_char[i] <= 'F' && content_char[i] >= 'A') || (content_char[i] <= 'f' && content_char[i] >= 'a') ||(content_char[i] <= '9' && content_char[i] >= '0') )
                {
                    result += content_char[i];
                }
            }
            if (k != content_List.size() -1)
            {
                result += '\n';
            }

            //  将获取的内容用append输出到TextBrowser
            // 将结果输出到结果区域
        }
        style_result = result;
        //qDebug() << result;
    }
    void mac_format(int spli_num, QString separator)
    {
        QString mode_result = "";
        //mode2_result = "";
        QString content = result; //读取左边区域数据
        content_List = content.split('\n');
        for (int k = 0 ; k < content_List.size();k++)
        {
            //char *content_char = Line.toLatin1().data(); // 将QString转化为char字符数组
            byteTmp = content_List[k].toLatin1();
            content_char = byteTmp.data();
            int j = 0;
            for (int i = 0; i < content_List[k].size(); i++)
            {

                if (j % spli_num == (spli_num - 1) && j != 11)
                {
                    mode_result += content_char[i];
                    mode_result += separator;
                }
                else
                {
                    mode_result += content_char[i];
                }
                j++;
            }
            if (k != content_List.size() -1)
            {
                mode_result += '\n';
            }

        }
        //  将获取的内容用append输出到TextBrowser
        style_result = mode_result;
        //输出结果到txtotpt区域
        printResult(style_result);
    }


};




#endif // MAC_CALC_H
