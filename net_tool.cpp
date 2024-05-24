#include "net_tool.h"
#include "ui_net_tool.h"






net_tool::net_tool(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::net_tool)
{
    ui->setupUi(this);
    ui->tab_select->setAttribute(Qt::WA_StyledBackground);
    ui->tab_select->setStyleSheet("QTabWidget#tab_select{background-color:rgb(34,34,34);border-top-color:rgb(34,34,34)}\
                                    QTabBar::tab{background-color:rgb(34,34,34);color:rgb(255,255,255);font:11pt 'Microsoft YaHei UI'}\
                                    QTabBar::tab::selected{background-color:rgb(100,100,100);color:rgb(0,0,0);font:11pt 'Microsoft YaHei UI'}");
    ui->txtitpt->setPlaceholderText("每行输入一个MAC");
    ui->net_input->setPlaceholderText("计算网络地址，需要每行输入一个需要计算的地址段，例如：\n10.1.1.1/8\n8.8.8.8 255.255.255.252\n\nIP和INT转换每行输入IP地址或数字即可");
    connect(ui->print_Upper, SIGNAL(clicked()), this, SLOT(print_Upper()));
    connect(ui->print_Lower, SIGNAL(clicked()), this, SLOT(print_Lower()));
    connect(ui->mode1, SIGNAL(clicked()), this, SLOT(mode1()));
    connect(ui->mode2, SIGNAL(clicked()), this, SLOT(mode2()));
    connect(ui->mode3, SIGNAL(clicked()), this, SLOT(mode3()));
    connect(ui->mode4, SIGNAL(clicked()), this, SLOT(mode4()));

//---------------------------base_convertion--------------------------
    QRegularExpression base_2("[0-1]{1,63}");
    QValidator *base_2_validator = new QRegularExpressionValidator(base_2,ui->conver22);
    ui->conver22->setValidator(base_2_validator);
    //2进制区域只允许输入0和1
    QRegularExpression base_8("[0-7]*");
    QValidator *base_8_validator = new QRegularExpressionValidator(base_8,ui->conver28);
    ui->conver28->setValidator(base_8_validator);
    //8进制区域只允许输入0至7
    QRegularExpression base_10("[0-9]*");
    QValidator *base_10_validator = new QRegularExpressionValidator(base_10,ui->conver210);
    ui->conver210->setValidator(base_10_validator);
    //10进制区域只允许输入0至9
    QRegularExpression base_16("[0-9a-fA-F]*");
    QValidator *base_16_validator = new QRegularExpressionValidator(base_16,ui->conver216);
    ui->conver216->setValidator(base_16_validator);
    //16进制区域只允许输入0至f
}
//---------------------------base_convertion--------------------------


net_tool::~net_tool()
{
    delete ui;
}
//-----------------------------mac_calc-------------------------------------------

void net_tool::To_upper()
{
    result = result.toUpper();
    style_result = style_result.toUpper();
}

void net_tool::To_lower()
{
    result = result.toLower();
    style_result = style_result.toLower();
}

void net_tool::printResult(QString mode_result)
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

void net_tool::read_mac_vendor()
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

void net_tool::calc_mac_vendor()
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

void net_tool::print_Upper()
{
    net_tool::To_upper();
    net_tool::printResult(style_result);
}

void net_tool::print_Lower()
{
    net_tool::To_lower();
    net_tool::printResult(style_result);
}


void net_tool::mac_format(int spli_num , QString separator)
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



void net_tool::mode1()
{
    style_result = result;
    printResult(style_result);
}

void net_tool::mode2()
{
    mac_format(4,".");
}

void net_tool::mode3()
{
    mac_format(2,":");
}

void net_tool::mode4()
{
    spli_num = ui->spli_num_box->value();
    separator = ui->spli_char_box->displayText();
    mac_format(spli_num,separator);
}

void net_tool::on_clean_clicked()
{
    // 清除结果区域前面缓存的结果
    ui->txtotpt->clear();
    ui->txtitpt->clear();
    result = "";
}

void net_tool::on_Vendor_id_stateChanged()
{
    if (ui->Vendor_id->isChecked())
    {
        Vendor_id_tag = true;
        if (Vendor_read == false)
            net_tool::read_mac_vendor();
        Vendor_read = true;
        //qDebug("yes,checked");
    }
    else
    {
        Vendor_id_tag = false;
        //qDebug("no!");
    }
}

void net_tool::on_txtitpt_textChanged()
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


//--------------------ip_source--------------------------------------------------------------------------------------


// void net_tool::on_ip_Input_textChanged()
// {
//     // 清除结果区域前面缓存的结果
//     //ui->txtotpt->clear();
//     read_ip = "";
//     //  获取输入框的内容
//     tmp_read_ip = ui->ip_Input->toPlainText(); //读取左边区域数据
//     //ui->txtitpt->append(content);
//     read_ip_list = tmp_read_ip.split('\n');
//     //qDebug() << read_ip_list;
//     for (int k = 0; k < read_ip_list.size(); ++k) {

// //        if (read_ip_list[k] == "")
// //            continue;

//         byteTmp = read_ip_list[k].toLatin1();
//         //qDebug() << read_ip_list[k];
//         ip_format = byteTmp.data(); // 将QString转化为char字符数组
//         //qDebug() << byteTmp.data();
//         for (int i = 0; i < read_ip_list[k].size() ; i++) //获取输入的MAC并转为小写无字符到result
//         {
//             //qDebug() << content_char[i];

//             if ((ip_format[i] <= '9' && ip_format[i] >= '0') || ip_format[i] == '.')
//             {
//                 //qDebug() << ip_format[i];
//                 read_ip += ip_format[i];
//             }
//         }
//         if (k != (read_ip_list.size() -1) && read_ip_list[k] != "")
//         {
//             read_ip += '\n';
//         }

//         //  将获取的内容用append输出到TextBrowser
//         // 将结果输出到结果区域
//         }
//     read_ip_list = read_ip.split('\n');
//     //qDebug() << read_ip_list;
//     //qDebug() << read_ip;
//     //conDB();
// }

// void net_tool::read_ip_merge()
// {
//     if (ip_merge_is_read == false)
//     {
//         QFile readFile(":/data/ip_merge.csv");
//         if (!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
//             return;
//         uchar* fPtr = readFile.map(0,readFile.size());
//         //qDebug() << fPtr;
//         //QTextStream *tmp_csv = new QTextStream(&readFile);
//         //qDebug() << &tmp_csv;
//         //qDebug() << tmp_csv;
//         QString memory_read = (char*)fPtr;
//         QStringList tmp_lines = memory_read.split("\r\n");
//     //    ip_merge.append(tmp_lines[1].split(","));
//         //qDebug() << tmp_lines;

//         //QMessageBox::warning(0,"test",tmp_lines[30000],QMessageBox::Yes);

//         for (int i = 0; i < tmp_lines.count(); i++)
//         {
//            ip_merge.append(tmp_lines.at(i).split(","));

//         }
//         //qDebug() << mac_vendor[10844][1] << '\t' << mac_vendor[3086][2];

//         //test
// //        for ( int i = 0 ; i < ip_merge.size();i++)
// //        {
// //            ip_head.append(ip_merge[i][0].toUInt());
// //        }



//         readFile.close();
//         fPtr = nullptr;
//         ip_merge_is_read = true;
//     }


// }


// int net_tool::lower_bound(QVector<QVector<QString>> ip_merge ,unsigned int target)
// {
//     int start = 0,end = ip_merge.size();
//     while(start < end)
//     {
//         int mid = start + (end - start)/2;
//         if (ip_merge[mid][0].toUInt() <= target)
//         {
//             start = mid+1;
//         }
//         else
//             end = mid;

//     }
//     return start;
// }

// void net_tool::ip_source_Search()
// {
// //    sql_search = "";
// //    country.clear();
// //    province.clear();
// //    city.clear();
// //    isp.clear();
//     QVector<QString>().swap(country);
//     QVector<QString>().swap(province);
//     QVector<QString>().swap(city);
//     QVector<QString>().swap(isp);
//     //qDebug() << country.size();

//     for(int i = 0; i < int_Ip.size();i++)
//     {
//         int search_path = lower_bound(ip_merge,int_Ip[i])-1;
//         country.append(ip_merge[search_path][2]);
//         province.append(ip_merge[search_path][3]);
//         city.append(ip_merge[search_path][4]);
//         isp.append(ip_merge[search_path][5]);
//     }

//     ip_final_result = "";
//     for (int k = 0 ; k < country.size();k++)
//     {
//         int ip_len = 16 - read_ip_list[k].length();
//         QString space_input = "";
//         for(int j = 0 ; j <= ip_len;j++)
//         {
//             space_input = space_input + " ";
//         }

//         if (country[k] != "0")
//             ip_final_result =ip_final_result +  read_ip_list[k] + space_input + "\t" + country[k];
//         if (province[k] != "0")
//             ip_final_result =ip_final_result + '\t' + province[k];
//         if(city[k] != "0")
//             ip_final_result =ip_final_result + '\t' + city[k];
//         if(isp[k] != "0")
//             ip_final_result =ip_final_result + '\t' + isp[k] + '\n';
//         else if (k != country.size()-1)
//             ip_final_result =ip_final_result + '\n';
//     }
//     ui->ip_Output->clear();
//     ui->ip_Output->append(ip_final_result);
//     //db.close();
//     //qDebug() << sql_search;
// }


// void net_tool::on_clear_ip_source_clicked()
// {
//     ui->ip_Input->clear();
//     ui->ip_Output->clear();
// }


// void net_tool::on_clear_ip_memory_clicked()
// {
//     QVector<QVector<QString>>().swap(ip_merge);
//     ip_merge_is_read = false;
// }



// void net_tool::on_ip_Search_clicked()
// {
//     read_ip_merge();
//     ipToint();
// //    conDB();
//     ip_source_Search();
// }



//-------------------base_convertion---------------------

QString net_tool::conver_1022(QString val)
{
    unsigned long long j = val.toULongLong(), i = 1;
    QString value = "";
    //qDebug() << j;
    while (true)
    {
        if (j<2)
        {
            value = QString::number(j) + value;
            break;
        }
        else if (j>=i)
        {
            value = QString::number((j/i)%2) + value;
        }
        else
        {
            break;
        }
        i*=2;
    }
    //qDebug() << value;
    return value;
}

QString net_tool::conver_1028(QString val)
{
    unsigned long long j = val.toULongLong(), i = 1;
    QString value = "";
    //qDebug() << j;
    while (true)
    {
        if (j<8)
        {
            value = QString::number(j) + value;
            break;
        }
        else if (j>=i)
        {
            value = QString::number((j/i)%8) + value;
        }
        else
        {
            break;
        }
        i*=8;
    }
    //qDebug() << value;
    return value;
}

QString net_tool::conver_10216(QString val)
{
    unsigned long long j = val.toULongLong();
    QString value = QString::number(j,16);
    return value;
}


unsigned long long net_tool::conver_check(unsigned long long val)
{
    if (val > 9223372036854775807 )
    {
        QMessageBox::warning(this,
                tr("计算结果超过阈值！"),
                tr("计算结果超过阈值，请减少计算的位数。"),
                QMessageBox::Ok, QMessageBox::Ok);
        val = 0;
    }
    return val;
}



unsigned long long net_tool::conver_2210(QString val)
{
    unsigned long long j = 1,value = 0;
    int len = val.length() - 1;
    for(int i = 0;i <= len ;i++)
    {
        value += val.mid(len - i,1).toULongLong() *j;
        j*=2;
    }
    return value;
}

unsigned long long net_tool::conver_8210(QString val)
{
    unsigned long long j = 1,value = 0;
    int len = val.length() - 1;
    for(int i = 0;i <= len ;i++)
    {
        value += val.mid(len - i,1).toULongLong() *j;
        j*=8;
    }
    value = conver_check(value);
    return value;
}

unsigned long long net_tool::conver_16210(QString val)
{
    bool ok;
    unsigned long long value = val.toULongLong(&ok,16);
    value = conver_check(value);
    return value;
}


void net_tool::on_conver22_textEdited(const QString &arg1)
{
    base2_txt = ui->conver22->text();
    tmp_base10 = conver_2210(base2_txt);
    QString tmp = QString::number(tmp_base10);
    ui->conver210->setText(QString::number(tmp_base10));
    ui->conver28->setText(conver_1028(tmp));
    ui->conver216->setText(conver_10216(tmp));
}


void net_tool::on_conver28_textEdited(const QString &arg1)
{
    base8_txt = ui->conver28->text();
    tmp_base10 = conver_8210(base8_txt);
    QString tmp = QString::number(tmp_base10);
    ui->conver210->setText(QString::number(tmp_base10));
    ui->conver22->setText(conver_1022(tmp));
    ui->conver216->setText(conver_10216(tmp));
}


void net_tool::on_conver210_textEdited(const QString &arg1)
{
    base10_txt = ui->conver210->text();
    QString tmp = QString::number(conver_check(base10_txt.toULongLong()));
    ui->conver28->setText(conver_1028(tmp));
    ui->conver22->setText(conver_1022(tmp));
    ui->conver216->setText(conver_10216(tmp));
}


void net_tool::on_conver216_textEdited(const QString &arg1)
{
    base16_txt = ui->conver216->text();
    tmp_base10 = conver_16210(base16_txt);
    QString tmp = QString::number(tmp_base10);
    ui->conver210->setText(QString::number(tmp_base10));
    ui->conver22->setText(conver_1022(tmp));
    ui->conver28->setText(conver_1028(tmp));
}





//-------------------net_calc---------------------

void net_tool::net_check(QStringList &net_txt_list)
{
    QString net_address = "";
    QString pattern1 = " *(\\d+\\.\\d+\\.\\d+\\.\\d+)[ \\/]{1,100}(\\d+) *";
    QString pattern2 = " *(\\d+\\.\\d+\\.\\d+\\.\\d+)[ \\/]{1,100}(\\d+\\.\\d+\\.\\d+\\.\\d+) *";
    QRegularExpression ip_tmp1(pattern1);
    QRegularExpression ip_tmp2(pattern2);
    QStringList num_tmp;


    for (int i = 0; i < (net_txt_list.length()); ++i) {
        int yanma = 0,yanma_tmp = 0;
        QRegularExpressionMatch ip_match1 = ip_tmp1.match(net_txt_list[i]);
        QRegularExpressionMatch ip_match2 = ip_tmp2.match(net_txt_list[i]);
        num_tmp.clear();
        if(ip_match2.hasMatch())
        {
//            qDebug() << "test2";
//            qDebug() << ip_match2.captured(1) + '/' + ip_match2.captured(2) ;

            num_tmp = (ip_match2.captured(1) + '.' + ip_match2.captured(2)).split('.');
            if (!((num_tmp[7].toInt() <= num_tmp[6].toInt()) && (num_tmp[6].toInt() <= num_tmp[5].toInt()) && (num_tmp[5].toInt() <= num_tmp[4].toInt())))
            {
                net_txt_list[i] = "error";
                continue;
            }
            for (int j = 0; j < num_tmp.length(); ++j) {
                if ((num_tmp[j].toInt() > 255) || (num_tmp[j].toInt() < 0))
                {
                    net_txt_list[i] = "error";
                    break;
                }
                else if (j >= 4)
                {
                    if(!(num_tmp[j].toInt()==255 || num_tmp[j].toInt()==254||num_tmp[j].toInt()==252||num_tmp[j].toInt()==248||num_tmp[j].toInt()==240||num_tmp[j].toInt()==224||num_tmp[j].toInt()==192||num_tmp[j].toInt()==128||num_tmp[j].toInt()==0)){
                        net_txt_list[i] = "error";
                        break;
                    }
                }
            }
            if (net_txt_list[i] != "error")
            {
                for (int l = 4; l < 8; ++l) {
                    yanma_tmp = 8 - log2(256 - num_tmp[l].toInt());
                    yanma = yanma + yanma_tmp;
                }
                net_txt_list[i] = ip_match2.captured(1) + '/' + QString::number(yanma);
            }

        }
        else if (ip_match1.hasMatch())
        {
            num_tmp = ip_match1.captured(1).split('.');
            for (int k = 0; k < num_tmp.length(); ++k) {
                if ((num_tmp[k].toInt() > 255) || (num_tmp[k].toInt() < 0) || (ip_match1.captured(2).toInt() > 32) || (ip_match1.captured(2).toInt() < 0))
                {
                    net_txt_list[i] = "error";
                    break;
                }
            }
            if (net_txt_list[i] != "error")
            {
                net_txt_list[i] = ip_match1.captured(1) + '/' + ip_match1.captured(2);
            }
        }
        else
        {
            net_txt_list[i] = "error";
        }

    }
}


void net_tool::on_net_input_textChanged()
{
    net_calc_output_txt = "";
    net_txt = ui->net_input->toPlainText();//读取左边数据
    net_txt_list = net_txt.split('\n');

}




void net_tool::on_net_calc_clicked()
{
    net_check(net_txt_list);
    QStringList out_put_net = net_txt_list;
    for (int net_num = 0; net_num < out_put_net.length(); ++net_num) {
        if (out_put_net[net_num] == "error")
        {
            continue;
        }
        QStringList ip_list = out_put_net[net_num].split(QRegularExpression("[\\./]"));
        for (int ip_num = 0; ip_num < ip_list.length(); ++ip_num) {
            int A = ip_list[0].toInt(),B = ip_list[1].toInt(),C = ip_list[2].toInt(),D = ip_list[3].toInt(),E = ip_list[4].toInt();
            int mi = 1;
            if (E <= 8)
            {
                mi = pow(2, 8 - E); //计算2的 8-E
                A = ip_list[0].toInt() - ip_list[0].toInt() % mi;
                out_put_net[net_num] =  QString::number(A) + ".0.0.0/" + QString::number(E);
            }
            else if (E > 8 && E <= 16)
            {
                mi = pow(2, 16 - E);
                B = ip_list[1].toInt() - ip_list[1].toInt() % mi;
                out_put_net[net_num] = QString::number(A) + '.' + QString::number(B) + ".0.0/" + QString::number(E);
            }
            else if (E > 16 && E <= 24)
            {
                mi = pow(2, 24 - E);
                C = ip_list[2].toInt() - ip_list[2].toInt() % mi;
                out_put_net[net_num] = QString::number(A) + '.' + QString::number(B) + '.' + QString::number(C) + ".0/" + QString::number(E);
            }
            else if (E > 24 && E <= 32)
            {
                mi = pow(2, 32 - E);
                D = ip_list[3].toInt() - ip_list[3].toInt() % mi;
                out_put_net[net_num] = QString::number(A) + '.' + QString::number(B) + '.' + QString::number(C) + '.' + QString::number(D) + '/' + QString::number(E);
            }
        }
    }
    QString out_txt = "";
    for (int i = 0; i < out_put_net.length(); ++i) {
        out_txt = out_txt + out_put_net[i] + '\n';

    }
    ui ->net_Output->setText(out_txt);

}

void net_tool::on_net_range_clicked()
{
    net_check(net_txt_list);
    QStringList out_start_net = net_txt_list;
    QStringList out_end_net = net_txt_list;

    for (int net_num = 0; net_num < out_start_net.length(); ++net_num) {
        if (out_start_net[net_num] == "error") {
            continue;
        }

        static QRegularExpression regex("[\\./]");

        QStringList ip_list = out_start_net[net_num].split(regex);

        if (ip_list.length() < 5) {
            // 处理ip_list长度不足的情况
            continue;
        }

        int A = ip_list[0].toInt();
        int B = ip_list[1].toInt();
        int C = ip_list[2].toInt();
        int D = ip_list[3].toInt();
        int E = ip_list[4].toInt();

        // int mi = 1;


        if (E <= 8) {
            int mask = (1 << (8 - E)) - 1;
            A = A & ~mask;
            out_start_net[net_num] = QString::number(A) + ".0.0.0";
            out_end_net[net_num] = QString::number(A | mask) + ".255.255.255";
        } else if (E <= 16) {
            int mask = (1 << (8 - (E-8))) - 1;
            B = B & ~mask;
            out_start_net[net_num] = QString::number(A) + '.' + QString::number(B) + ".0.0";
            out_end_net[net_num] = QString::number(A) + '.' + QString::number(B | mask) + ".255.255";
        } else if (E <= 24) {
            int mask = (1 << (8 - (E-16))) - 1;
            C = C & ~mask;
            out_start_net[net_num] = QString::number(A) + '.' + QString::number(B) + '.' + QString::number(C) + ".0";
            out_end_net[net_num] = QString::number(A) + '.' + QString::number(B) + '.' + QString::number(C | mask) + ".255";
        } else if (E <= 32) {
            int mask = (1 << (8 - (E-24))) - 1;
            D = D & ~mask;
            out_start_net[net_num] = QString::number(A) + '.' + QString::number(B) + '.' + QString::number(C) + '.' + QString::number(D);
            out_end_net[net_num] = QString::number(A) + '.' + QString::number(B) + '.' + QString::number(C) + '.' + QString::number(D | mask);
        }

    }
    QString out_txt = "";
    QStringList address_group;
    for (int i = 0; i < out_start_net.length(); ++i) {
        out_txt = out_txt + out_start_net[i] + " ~ " + out_end_net[i]  + '\n';
    }
    ui ->net_Output->setText(out_txt);
}



void net_tool::on_net_mask_clicked()
{
    net_check(net_txt_list);
    QStringList out_start_net = net_txt_list;
    QStringList mask_list = net_txt_list;
    int mask = 0;
    for (int net_num = 0; net_num < out_start_net.length(); ++net_num) {
        if (out_start_net[net_num] == "error") {
            continue;
        }

        static QRegularExpression regex("[\\./]");

        QStringList ip_list = out_start_net[net_num].split(regex);

        if (ip_list.length() < 5) {
            // 处理ip_list长度不足的情况
            continue;
        }

        int A = ip_list[0].toInt();
        int B = ip_list[1].toInt();
        int C = ip_list[2].toInt();
        int D = ip_list[3].toInt();
        int E = ip_list[4].toInt();

        // int mi = 1;

        out_start_net[net_num] = QString::number(A) + "." + QString::number(B) + "." + QString::number(C) + "." + QString::number(D);

        if (E <= 8) {
            mask = (1 << (8 - E)) - 1;
            mask_list[net_num] = QString::number(255 - mask) + ".0.0.0";
        } else if (E <= 16) {
            mask = (1 << (8 - (E-8))) - 1;
            mask_list[net_num] = "255." +  QString::number(255 - mask) + ".0.0";
        } else if (E <= 24) {
            mask = (1 << (8 - (E-16))) - 1;
            mask_list[net_num] = "255.255." + QString::number(255 - mask) + ".0";
        } else if (E <= 32) {
            mask = (1 << (8 - (E-24))) - 1;
            mask_list[net_num] = "255.255.255." + QString::number(255 - mask);
        }

    }
    QString out_txt = "";
    // QStringList address_group;
    for (int i = 0; i < out_start_net.length(); ++i) {
        out_txt = out_txt + out_start_net[i] + " " + mask_list[i]  + '\n';
    }
    ui ->net_Output->setText(out_txt);


}


void net_tool::ipToint()
{

    //----IP转int----------------------------------------------------------
    QStringList out_put_net = net_txt_list;
    tmp = 0;
    QVector<unsigned int>().swap(int_Ip);
    for (int i = 0 ; i < out_put_net.size();i ++)
    {
        if (out_put_net[i] == "")
            continue;
        str_ip = out_put_net[i].split(".");
        //qDebug() << str_ip;
        if ( str_ip[0].length() > 3 || str_ip[1].length() > 3 || str_ip[2].length() > 3  || str_ip[3] .length() > 3 || str_ip.size() > 4)
            continue;
        tmp = (str_ip[0].toInt() << 24) + (str_ip[1].toInt() << 16) + (str_ip[2].toInt() << 8) + (str_ip[3].toInt());

        int_Ip.append(tmp);
    }
}

void net_tool::intToip()
{
    QStringList out_put_net = net_txt_list;
    QString tmp_intip = "";
    for(int i = 0 ; i < out_put_net.size();i++)
    {
        tmp_intip = tmp_intip +  QString::number(out_put_net[i].toUInt() / 16777216) + '.';
        tmp_intip = tmp_intip +  QString::number((out_put_net[i].toUInt() / 65536) % 256) + '.';
        tmp_intip = tmp_intip +  QString::number((out_put_net[i].toUInt() / 256) % 256) + '.';
        tmp_intip = tmp_intip +  QString::number(out_put_net[i].toUInt() % 256);
        if (i != out_put_net.size() - 1)
        {
            tmp_intip = tmp_intip + '\n';
        }

    }
    ui->net_Output->append(tmp_intip);
}

void net_tool::on_IPtoint_clicked()
{
    ipToint();
    QString tmp_int_ip = "";
    for (int i = 0 ; i < int_Ip.size();i++)
    {
        tmp_int_ip = tmp_int_ip +  QString::number(int_Ip[i]);
        if (i != int_Ip.size()-1)
        {
            tmp_int_ip = tmp_int_ip + '\n';
        }
    }
    ui->net_Output->clear();
    ui->net_Output->append(tmp_int_ip);
}


void net_tool::on_inttoIP_clicked()
{
    ui->net_Output->clear();
    intToip();
}





void net_tool::on_net_calc_clear_clicked()
{
    ui->net_input->clear();
    ui->net_Output->clear();
    net_txt_list.clear();
}

