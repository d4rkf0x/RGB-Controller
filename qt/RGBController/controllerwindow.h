#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

/* includes */

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QSerialPortInfo>
#include <QTextStream>
#include "serial_communication.h"
#include "ui_controllerwindow.h"

namespace Ui {
	class controllerWindow;
}

class controllerWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit controllerWindow(QWidget *parent = 0);
		~controllerWindow();

        /* public functions */
        void info_log(QString text);
        void populate_serial_list();
        void load_presets();
        void serial_rgb_change(int r, int g, int b);
        /*public variables */
        /* these three ints will hold the current value (0 - 255) of each slider */
        int r, g, b;
        /* this list will contain all the presets loaded from file */
        QStringList presets;
        /* this string will hold our selected serial port */
        QString port;
        /* this int will contain the current preset index selected */
        int preset_index;

private slots:
        /* these slots are used to trigger button clicks and drop down items selections etc */
        void on_connect_button_clicked();

        void on_disconnect_button_clicked();

        void on_refresh_port_button_clicked();

        void on_reload_preset_button_clicked();

        void on_r_slider_valueChanged(int value);

        void on_g_slider_valueChanged(int value);

        void on_b_slider_valueChanged(int value);

        void on_red_button_clicked();

        void on_green_button_clicked();

        void on_blue_button_clicked();

        void on_off_button_clicked();

        void on_set_preset_button_clicked();

        void on_presets_dropdown_currentIndexChanged(int index);

private:
		Ui::controllerWindow *ui;
        /* serial communication object */
        serial_communication portf;
};

#endif // CONTROLLERWINDOW_H