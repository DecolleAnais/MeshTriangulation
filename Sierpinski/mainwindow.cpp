#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->button_display_mode, SIGNAL(released()), this, SLOT(on_button_display_mode_released()));
    connect(ui->checkbox_voronoi_cells, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_voronoi_cells_stateChanged()));
    connect(ui->checkbox_voronoi_vertices, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_voronoi_vertices_stateChanged()));

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
