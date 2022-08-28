#include "converter.h"
#include "ui_converter.h"
#include "cmath"

Converter::Converter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Converter)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/new/icons/icon.ico"));
    ui->centralwidget->setStyleSheet(" #centralwidget {background: rgb(245,245,245)}");


    ui->menuBar->setStyleSheet("QMenuBar {background: rgb(216,234,249)} QMenuBar::item:selected {background: rgb(216,234,249); color: rgb(0,120,215)}");
    ui->menu->setStyleSheet("QMenu {background: rgb(216,234,249)}  QMenu::item:selected {background: rgb(216,234,249); color: rgb(0,120,215)}");

    ui->lineEdit_in ->setStyleSheet(" QLineEdit {background: rgb( 245,245,245)}");
    ui->lineEdit_in->setReadOnly(true);
    ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(245,245,245)}");

    ui->pushButton_clear->setStyleSheet(" QPushButton { background: rgb(0,120,215); color: rgb(245,245,245)} ");
    ui->pushButton_result->setStyleSheet(" QPushButton { background: rgb(0,120,215); color: rgb(245,245,245)}");

    QColor cTable(204,228,247);

    for (int i=0; i<ui->comboBox_from->count(); i++)
    {
        ui->comboBox_from->setItemData(i, QVariant(QBrush(cTable)), Qt::BackgroundRole);
        ui->comboBox_in->setItemData(i, QVariant(QBrush(cTable)), Qt::BackgroundRole);
    }

    connect( ui->comboBox_from, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){Changed_m(index);});
    connect( ui->comboBox_in, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){Changed_n(index);});
    ui->pushButton_result->setShortcut(Qt::Key_Return);// выполнение on_pushButton_clear_clicked() по кнопке enter

    si_n = si_1+si_0;
    si_m = si_1+si_0;

    QPalette pal = this->palette();

    pal.setColor(QPalette::Window, QRgb(0x121218));
    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));

    this->setPalette(pal);

    numErrors=0;

    R = 245; G =245; B =245;
    connect(tmr, SIGNAL(timeout()), this, SLOT(setLineColor()));
    tmr->start(150);
}

Converter::~Converter()
{
    delete ui;
}


void Converter::on_pushButton_clear_clicked()
{
    ui->lineEdit_from->clear();
    ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(245,245,245)}");
    ui->lineEdit_in->setStyleSheet(" QLineEdit {background: rgb(245,245,245)}");
    ui->lineEdit_in->clear();
    ui->lineEdit_from->setToolTip("");
}

void Converter::on_pushButton_result_clicked()
{
    auto number = ui->lineEdit_from->text();

    if(check(number))
    {
        ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(245,245,245)}");
        ui->lineEdit_from->setToolTip("");
        if (m == n)ui->lineEdit_in->setText(number);
        else if (m == 10) ui->lineEdit_in->setText(FromDecimal(number, n));
        else if (n == 10) ui->lineEdit_in->setText(InDecimal(number, m));
        else
        {
          ui->lineEdit_in->setText(FromDecimal(InDecimal(number, m), n));
        }

        if(number.length())
        HistoryList.append(number + si_m + " = " + ui->lineEdit_in->text() + si_n);
    }
    else
    {
        R = 238; G =122; B =118;
        ui->lineEdit_from->setToolTip("ошибка ввода!"+error_1+error_2+error_3);

        numErrors++;
        if(numErrors>3) Warning();
    }
}

void Converter::setLineColor()
{
    if (R<245)
    {
        ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb( "
                                         + QString::number(R++) + ","
                                         + QString::number(G+=(245-122)/7) + ","
                                         + QString::number(B+=(245-118)/7) + ")}");
    }
    else
    {
        ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(245,245,245)}");
    }
}


void Converter::Changed_n(int i)
{
    switch (i)
    {
    case 0: n=10; si_n = si_1 + si_0;
        break;
    case 1: n=2; si_n=si_2;
        break;
    case 2: n=8; si_n=si_8;
        break;
    case 3: n=16; si_n = si_1 + si_6;
    default:
        break;
    }
}
void Converter::Changed_m(int i)
{
    switch (i)
    {
    case 0: m=10; si_m= si_n = si_1 + si_0;
        break;
    case 1: m=2; si_m= si_2;
        break;
    case 2: m=8; si_m= si_8;
        break;
    case 3: m=16; si_m=si_n = si_1 + si_6;
    default:
        break;
    }
}

QString Converter::FromDecimal(QString inp, int n)
{
    int input =static_cast<int>(floor(inp.toDouble()));

    if(input==-2147483648)
        return "сам считай такие большие числа";

    int p = 0;//количество символов до запятой или точки

    for (int q = 0; q < inp.length(); q++)
    {
        if (inp[q] == '.' || inp[q] == ',') break;
        p++;
    }

    QString out_befor;
    if (p == 0) out_befor = '0';

    QString output;

    while (input)
    {
        int remains = input % n;
        input /= n;

        switch (remains)
        {
        case 10:
            output.insert(0,'A');
            break;
        case 11:
            output.insert(0, 'B');
            break;
        case 12:
            output.insert(0, 'C');
            break;
        case 13:
            output.insert(0, 'D');
            break;
        case 14:
            output.insert(0, 'E');
            break;
        case 15:
            output.insert(0, 'F');
            break;
        default:
            output.insert(0, ((int)'0') + remains);
            break;
        }
    }

    double input_after = inp.toDouble() - floor(inp.toDouble());

    int i = 0;//счетчик точности

    QString output_after;

    while (static_cast<bool>(input_after) && i<5)
    {
        int remains = static_cast<int>(input_after* n);

        input_after = input_after * n - remains;

        switch (remains)
        {
        case 10:
            output_after.push_back('A');
            break;
        case 11:
            output_after.push_back('B');
            break;
        case 12:
            output_after.push_back('C');
            break;
        case 13:
            output_after.push_back('D');
            break;
        case 14:
            output_after.push_back('E');
            break;
        case 15:
            output_after.push_back('F');
            break;
        default:
            output_after.push_back(((int)'0') + remains);
            break;
        }

        i++;
    }

    if(qHash(output_after)) return out_befor + output + '.' + output_after;
    return output;

}

QString Converter::InDecimal(QString &input, int f)
{
    int p = 0;//количество символов до запятой или точки

    for (int q = 0; q < input.length(); q++)
    {
        if (input[q] == '.' || input[q] == ',') break;
        p++;
    }

    QString out_befor;
    if (p == 0) out_befor = '0';

    double output = 0;
    for (int i = 0, j= p-1; i< input.length(); i++ , j--)
    {
        int buf ;

        switch (qHash(input[i]))
        {
        case '.': case ',': buf = 0; j++;
            break;
        case 'A': case 'a': buf = 10;
            break;
        case 'B': case 'b': buf = 11;
            break;
        case 'C': case 'c': buf = 12;
            break;
        case 'D': case 'd': buf = 13;
            break;
        case 'E': case 'e': buf = 14;
            break;
        case 'F': case 'f': buf = 15;
            break;
        default: buf = static_cast<int>(qHash(input[i]) - '0');
            break;
        }

        output += (buf)* pow(f, j);
    }

    return QString::number(output);
};

bool Converter:: chek_symbol(QString& str)
{
    int c = 0;//счетчик символов точки и запятой

    for (int q = 0; q < str.length(); q++)
    {
        if (str[q] == '.' || str[q] == ',')
        {
            if (c)
            {
                ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(238,172,168)}");
                error_1 ="\nпосле дробной части не может идти символ '.' или ','";
                return 0;
            }
            if (q == (str.length() - 1))
            {
                error_1 ="\nне записана дробная часть";
                ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(238,172,168)}");
                return 0;
            }
            c++;
        }

        if (str[q] == '[' || str[q] == ']' || str[q] == '/' || str[q] == '&' || str[q] == '?'||
            str[q] == ';' || str[q] == ':' || str[q] == '!' || str[q] == '>' || str[q] == '-'||
            str[q] == '+' || str[q] == '=' || str[q] == '_' || str[q] == ')' || str[q] == '('||
            str[q] == '*' || str[q] == '^' || str[q] == '%' || str[q] == '$' || str[q] == '#'||
            str[q] == '@' || str[q] == '~' || str[q] == '`')
            {
                ui->lineEdit_from->setStyleSheet(" QLineEdit {background: rgb(238,172,168)}");
                error_1 = "\nне допускаются символы кроме ',' и '.' .";
                return 0;
            }

    }
    return 1;
}

bool Converter:: chek_letters(QString& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') ||
            (str[i] >= "А" && str[i] <= "Я") || (str[i] >= "а" && str[i] <= "я")
            || str[i]==QString::fromLocal8Bit("Ё")[0] || str[i]==QString::fromLocal8Bit("ё")[0])
        {
            error_2 = "\nв числе могут быть только цифры.";
            return 0;
        }
    }
    return 1;
}

bool Converter:: check(QString &str)
{
    bool f=1;

    if (m == 2)
    {
        for (int i = 0; i <str.length(); i++)
        {
            if (str[i] >= '2' && str[i] <= '9')
            {
                error_3 = "\nв двоичном числе могут быть только цифры 1 и 0.";
                f=0;
                break;
            }
        }

        if (!chek_letters(str)) f = 0;

        if (!chek_symbol(str)) f = 0;
    }

    else if (m == 8)
    {
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == '8' || str[i] == '9')
            {
                error_3 = "\nв восьмиричном числе могут быть только цифры 0-7.";
                f = 0;
                break;
            }
        }

        if (!chek_letters(str)) f = 0;

        if (!chek_symbol(str)) f = 0;
    }

    else if (m == 10)
    {
        //for (int i = 0; i < str.length(); i++)
        //{
            if (!chek_letters(str)) f = 0;

            if (!chek_symbol(str)) f = 0;
        //}
    }

    else if (m == 16)
    {
        for (int i = 0; i < str.length(); i++)
        {
            if ((str[i] >= 'g' && str[i] <= 'z') || (str[i] >= 'g' && str[i] <= 'z') ||
                (str[i] >= QString::fromLocal8Bit("А")[0] && str[i] <= QString::fromLocal8Bit("Я")[0]) ||
                (str[i] >= QString::fromLocal8Bit("а")[0] && str[i] <= QString::fromLocal8Bit("я")[0]))
            {
                error_3 = "\nв числе могут быть только цифры и латинские буквы от a до f.";
                f = 0;
                break;
            }
        }

        if (!chek_symbol(str)) f = 0;
    }
    return f;
}

void Converter::on_action_history_triggered()
{


    history.setWindowTitle("История операций");
    history.setWindowIcon(QIcon(":/new/icons/icon.ico"));
    history.setStyleSheet(" QDialog {background: rgb(216,234,249)}");

    scrollArea->setWidgetResizable(true);
    //scrollArea->setFixedSize(220, 200);

    while (k<HistoryList.count())
    {
        QLabel *lbl = new QLabel;//(" " + QString::number(k+1) + ".  " + HistoryList[k]);
        lbl->setText(" " + QString::number(k+1) + ".  " + HistoryList[k]);
        lbl->setFixedHeight(30);

        if(k%2) lbl->setStyleSheet(" QLabel { background: rgb(204,228,247); font-size: 20px}");
        else lbl->setStyleSheet(" QLabel { background: rgb(255,255,255); font-size: 20px}");
        pLabelLayout->addWidget(lbl, 1, Qt::AlignTop);
        k++;
    }

    pLabelLayout->setSpacing(1);

    scrollArea->setWidget(scrollAreaWidgetContents);

    cleen.setText("Очистить историю");
    exit.setText("Закрыть");

    cleen.setStyleSheet(" QPushButton { background: rgb(0,120,215); color: rgb(255,255,255)}");
    exit.setStyleSheet(" QPushButton { background: rgb(0,120,215); color: rgb(255,255,255)}");
    history.connect( &cleen, SIGNAL( clicked() ), this, SLOT( OnViewCleen() ) );
    history.connect( &exit, SIGNAL( clicked() ), this, SLOT( OnViewClose() ) );

    pButtonLayout.addWidget( &cleen );
    pButtonLayout.addWidget( &exit );

    vlayout.addLayout(&pButtonLayout);
    vlayout.addWidget(scrollArea);

    history.setLayout( &vlayout );

    history.adjustSize();
    history.show();
}

void  Converter::OnViewCleen()
{
    k=0;

    for(int i=0; pLabelLayout->count(); ++i)
    {
        QLayoutItem * item = pLabelLayout->itemAt((i));

        while(item=pLabelLayout->itemAt((0)))
        {
            HistoryList.pop_front();

            pLabelLayout->removeItem(item);
            pLabelLayout->removeWidget((item->widget()));
            delete item->widget();
            delete item;
            pLabelLayout->update();
        }
    }
}
void  Converter::OnViewClose()
{
    history.close();
}

void Converter::on_action_reference_triggered()
{
    numErrors=0;

    reference.setWindowTitle("Теоретическая справка");
    reference.setWindowIcon(QIcon(":/new/icons/icon.ico"));

    reference.setFixedSize(650, 400);

    lb.setAlignment(Qt::AlignCenter);
    QFile file("System.txt");
    QByteArray data;
    if(!file.open(QIODevice::ReadOnly))
    {
        lb.setText("Ошибка\nФайл не найден");
    }
    else
    {
        data = file.readAll();
        lb.setText(data);
    }

    lb1.setAlignment(Qt::AlignCenter);
    QFile file1("InDec.txt");
    QByteArray data1;
    if(!file1.open(QIODevice::ReadOnly))
    {
        lb1.setText("Ошибка\nФайл не найден");
    }
    else
    {
        data1 = file1.readAll();
        lb1.setText(data1);
    }

    lb2.setAlignment(Qt::AlignCenter);
    QFile file2("FromDec.txt");
    QByteArray data2;
    if(!file2.open(QIODevice::ReadOnly))
    {
        lb2.setText("Ошибка\nФайл не найден");
    }
    else
    {
        data2 = file2.readAll();
        lb2.setText(data2);
    }

    lb3.setAlignment(Qt::AlignCenter);
    QFile file3("Different.txt");
    QByteArray data3;
    if(!file3.open(QIODevice::ReadOnly))
    {
        lb3.setText("Ошибка\nФайл не найден");
    }
    else
    {
        data3 = file3.readAll();
        lb3.setText(data3);
    }

    tabWidget.addTab(&lb, tr("Системы счисления"));
    tabWidget.addTab(&lb1, tr("Перевод в десятичную систему"));
    tabWidget.addTab(&lb2, tr("Перевод из десятичной системы"));
    tabWidget.addTab(&lb3, tr("Перевод для произвольных систем"));

    verticalLayout.addWidget(&tabWidget);

    reference.setLayout(&verticalLayout );

    reference.adjustSize();
    reference.show();
}

void Converter::on_action_information_triggered()
{
    QFile file("README.txt");
    QString A;
    QByteArray data;
    if(!file.open(QIODevice::ReadOnly))
    {
        A = "Ошибка\nФайл не найден";
    }
    else
    {
        data = file.readAll();
        A = data;
    }

     QString About= "Программа предназначена для изучения переводов между различными системами счисления, может использоваться как простой конвертер.\n";
     About += "Обратная связь: elizavetinas@yandex.ru\n";
     About += "Москва 2022г.";

     QMessageBox MB(("О программе"),A,QMessageBox::NoIcon,QMessageBox::Ok,Qt::NoButton,Qt::NoButton);

     MB.setWindowIcon(QIcon(":/new/icons/icon.ico"));

     MB.setButtonText(QMessageBox::Ok,("ОК"));
     MB.setModal(true);

     MB.exec();
}

void Converter::Warning()
{
    warning.setWindowTitle("Предупреждение");
    warning.setWindowIcon(QIcon(":/new/icons/icon.ico"));

    warning.setFixedSize(300, 100);
    warning.setStyleSheet(" QDialog {background: rgb(216,234,249)}");

    lableW.setText("Прочитайте теоретическую справку о переводах\nмежду различными системами счисления");
    lableW.setAlignment(Qt::AlignCenter);
    verticalLayoutW.addWidget(&lableW);

    go.setText("Перейти"); //перейти к теоретической справке
    go.setStyleSheet(" QPushButton { background: rgb(0,120,215); color: rgb(255,255,255)}");
    warning.connect( &go, SIGNAL( clicked() ), this, SLOT( on_action_reference_triggered() ) );
    verticalLayoutW.addWidget(&go);

    warning.setLayout(&verticalLayoutW);

    warning.adjustSize();
    warning.show();
}
void Converter::on_action_close_triggered()
{
    this->close();
}
