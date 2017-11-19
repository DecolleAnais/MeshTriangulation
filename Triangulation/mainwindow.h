#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    
private slots:

    void on_button_display_mode_released();

    void on_checkbox_voronoi_cells_stateChanged();

    void on_checkbox_voronoi_vertices_stateChanged();

    void on_action_open_triggered();

    void on_action_save_as_triggered();

    void on_button_crust_released();

    void on_button_reset_released();

    void on_button_lawson_released();

    void on_checkbox_triangulation_stateChanged();

private:
    Ui::MainWindow *ui;
    QString currentFile;
};

#endif // MAINWINDOW_H
