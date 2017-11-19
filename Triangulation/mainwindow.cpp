#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentFile("")
{
    ui->setupUi(this);

    ui->button_crust->setEnabled(false);
    ui->button_display_mode->setEnabled(false);
    ui->button_lawson->setEnabled(false);
    ui->button_reset->setEnabled(false);
    ui->checkbox_voronoi_cells->setEnabled(false);
    ui->checkbox_voronoi_vertices->setEnabled(false);
    ui->checkbox_triangulation->setEnabled(false);
    ui->checkbox_triangulation->setChecked(true);

    connect(ui->button_display_mode, SIGNAL(released()), this, SLOT(on_button_display_mode_released()), Qt::UniqueConnection);
    connect(ui->button_crust, SIGNAL(released()), this, SLOT(on_button_crust_released()), Qt::UniqueConnection);
    connect(ui->button_lawson, SIGNAL(released()), this, SLOT(on_button_lawson_released()), Qt::UniqueConnection);
    connect(ui->checkbox_voronoi_cells, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_voronoi_cells_stateChanged()), Qt::UniqueConnection);
    connect(ui->checkbox_voronoi_vertices, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_voronoi_vertices_stateChanged()), Qt::UniqueConnection);
    connect(ui->checkbox_triangulation, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_triangulation_stateChanged()), Qt::UniqueConnection);
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_action_open_triggered()), Qt::UniqueConnection);
    connect(ui->actionSave_As, SIGNAL(triggered(bool)), this, SLOT(on_action_save_as_triggered()), Qt::UniqueConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_display_mode_released()
{
    ui->widget->changeDisplayMode();
}

void MainWindow::on_checkbox_voronoi_cells_stateChanged()
{
    ui->widget->setDisplayVoronoiCells( ui->checkbox_voronoi_cells->isChecked() );
}

void MainWindow::on_checkbox_voronoi_vertices_stateChanged()
{
    ui->widget->setDisplayVoronoiVertices( ui->checkbox_voronoi_vertices->isChecked() );
}

void MainWindow::on_checkbox_triangulation_stateChanged()
{
    ui->widget->setDisplayTriangulation(ui->checkbox_triangulation->isChecked() );
}

void MainWindow::on_action_open_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"Error","File not opened.");
        currentFile = "";
        return;
    }
    QTextStream stream(&file);
    ui->widget->loadFile(stream);
    file.close();
    ui->button_crust->setEnabled(true);
    ui->button_display_mode->setEnabled(true);
    ui->button_lawson->setEnabled(true);
    ui->button_reset->setEnabled(true);
    ui->checkbox_voronoi_cells->setEnabled(true);
    ui->checkbox_voronoi_vertices->setEnabled(true);
    ui->checkbox_triangulation->setEnabled(true);
    ui->checkbox_triangulation->setChecked(true);
}

void MainWindow::on_button_crust_released()
{
    ui->widget->applyCrust();
    ui->button_crust->setEnabled(false);
}

void MainWindow::on_action_save_as_triggered() {
    if(currentFile == "") {
        QMessageBox::warning(this,"Impossible","No file to save.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,"Error","File not opened.");
        return;
    }
    QTextStream out(&file);
    ui->widget->saveAs(out);
    file.flush();
    file.close();
}

void MainWindow::on_button_reset_released()
{
    currentFile = "";
    ui->widget->reset();
    ui->button_crust->setEnabled(false);
    ui->button_display_mode->setEnabled(false);
    ui->button_lawson->setEnabled(false);
    ui->button_reset->setEnabled(false);
    ui->checkbox_voronoi_cells->setEnabled(false);
    ui->checkbox_voronoi_vertices->setEnabled(false);
    ui->checkbox_triangulation->setEnabled(false);
    ui->checkbox_triangulation->setChecked(true);
}

void MainWindow::on_button_lawson_released()
{
    ui->widget->lawson();
}
