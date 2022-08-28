#ifndef CONVERTER_H
#define CONVERTER_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLayout>
#include <QMessageBox>
#include <QList>
#include <QPushButton>
#include <QTimer>
#include <QFile>
#include <QTabWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class Converter; }
QT_END_NAMESPACE

class Converter : public QMainWindow
{
    Q_OBJECT

public:
    Converter(QWidget *parent = nullptr);
    ~Converter();

    int n=10, m=10; //выходящая СИ и входящая СИ

    QString error_1="", error_2="", error_3=""; //набор ошибок ввода
    QString si_0=u8"\u2080", si_1=u8"\u2081" , si_2=u8"\u2082" , si_8=u8"\u2088" , si_6=u8"\u2086", si_n, si_m ; //индексы си

    QString l="";

    void Changed_n(int i); //переопределения системы счисления выводимого числа
    void Changed_m(int i); //переопределения системы счисления вводимого числа

    QString FromDecimal(QString inp, int n); //функция перевода из десятичной СИ
    QString InDecimal(QString &input, int f); //функция перевода в десятичную СИ

    bool chek_symbol(QString &str); //проверка на символы во вводимом числе
    bool chek_letters(QString& str); //проверка на буквы во вводимом числе
    bool check(QString &str); //проверка вводимого числа

    QList <QString> HistoryList; //контейнер для истории операций

    int R, G, B;

    int k=0;

    int numErrors;

private slots:

    void on_pushButton_clear_clicked(); //кнопка очистки полей главного окна

    void on_pushButton_result_clicked(); //кнопка расчета преобразования

    void on_action_history_triggered(); //открытие окна истории преобразований

    void on_action_reference_triggered(); //открытие окна теоретических справок

    void on_action_information_triggered(); //открытие окна О программе

    void on_action_close_triggered(); //закрытие приложения

    void setLineColor(); //изменение цвета при ошибке

    void  Warning(); //предложение изучить теоретическую справку

    void  OnViewCleen(); //очистить историю операций
    void  OnViewClose(); //закрыть историю


public slots:


private:

    Ui::Converter *ui;

    QDialog history; //история конвертора
    QVBoxLayout vlayout;
    QHBoxLayout pButtonLayout;//компоновка кнопок истории операций
    QPushButton cleen;
    QPushButton exit;
    QScrollArea *scrollArea = new QScrollArea(&history); //прогрутка длинной истории операций
    QWidget *scrollAreaWidgetContents = new QWidget(scrollArea); //виджет, где лежит прокрутка
    QVBoxLayout *pLabelLayout = new QVBoxLayout(scrollAreaWidgetContents);

    QDialog reference; //справочный материал по системам счисления и конвертации
    QVBoxLayout verticalLayout;
    QTabWidget tabWidget;
    QLabel lb ;
    QLabel lb1 ;
    QLabel lb2 ;
    QLabel lb3 ;

    QDialog inform; //информация о программе, вид лицензии

    QDialog warning; //предупреждение, что много ошибок ввода
    QVBoxLayout verticalLayoutW;
    QLabel lableW;
    QPushButton go;

    QTimer * tmr = new QTimer(this);
};
#endif // CONVERTER_H

//**********************************************************\\
//Теоретическая справка(сделать через html или чет типа того)


