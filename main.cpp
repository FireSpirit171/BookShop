#include "MainWindow/mainwindow.h"
#include "UserWindow/userwindow.h"
#include "EmployeeWindow/employeewindow.h"
#include "SETTINGS/SETTINGS.h"

#include <string>
#include <iostream>

#include <QLocale>
#include <QTranslator>
#include <QApplication>
#include <QtSql>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName(SETTINGS::get_host_name());
    db.setPort(SETTINGS::get_port());
    db.setDatabaseName(SETTINGS::get_database_name());
    db.setUserName(SETTINGS::get_user_name());
    db.setPassword(SETTINGS::get_password());

    if (db.open()) {
        qDebug() << "Connected to database";
        MainWindow w;

        // Обработчик события "Войти"
        QObject::connect(&w, &MainWindow::loginButtonClicked, [&]() {
            QString role = w.getRole();
            if (role == ""){
                QMessageBox::warning(&w, "Ошибка.", "Выберите роль!");
                return; // Возвращаемся из обработчика, не продолжая выполнение кода
            }

            // Получение введенного логина и пароля
            QString login = w.getLogin();
            QString password = w.getPassword();

            // Формирование SQL-запроса с вставкой значений
            QString queryStr = "SELECT COUNT(*) FROM \"Users\" WHERE \"UserRole\" = :role AND \"Login\" = :login AND \"Password\" = :password";

            // Подготовка SQL-запроса
            QSqlQuery query;
            query.prepare(queryStr);
            query.bindValue(":role", role);
            query.bindValue(":login", login);
            query.bindValue(":password", password);

            // Выполнение запроса
            if (query.exec()) {
                if (query.next()) {
                    int count = query.value(0).toInt();
                    if (count > 0) {
                        // Если пользователь найден, определяем роль и открываем соответствующее окно
                        w.hide();
                        if (role == "Покупатель") {
                            UserWindow userWindow;
                            QObject::connect(&userWindow, &UserWindow::finished, [&w]() {
                                w.show(); // Отображаем главное окно при закрытии EmployeeWindow
                            });
                            userWindow.exec();
                        } else if (role == "Сотрудник") {
                            EmployeeWindow employeeWindow;
                            QObject::connect(&employeeWindow, &EmployeeWindow::finished, [&w]() {
                                w.show(); // Отображаем главное окно при закрытии EmployeeWindow
                            });
                            employeeWindow.exec();
                        }
                    } else {
                        // Если пользователь не найден, выводим сообщение об ошибке
                        QMessageBox::warning(&w, "Ошибка авторизации", "Неверный логин или пароль!");
                    }
                }
            } else {
                // Вывод сообщения об ошибке в случае неудачи выполнения запроса
                qDebug() << "Query execution failed:" << query.lastError().text();
            }



        });

        w.show();
        return a.exec();

    } else {
        qDebug() << "Failed to connect to database:" << db.lastError().text();
    }

    return 0;
}
