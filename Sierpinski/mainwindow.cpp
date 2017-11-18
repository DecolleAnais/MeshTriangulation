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
    connect(ui->button_display_mode, SIGNAL(released()), this, SLOT(on_button_display_mode_released()));
    connect(ui->button_crust, SIGNAL(released()), this, SLOT(on_button_crust_released()));
    connect(ui->checkbox_voronoi_cells, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_voronoi_cells_stateChanged()));
    connect(ui->checkbox_voronoi_vertices, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_voronoi_vertices_stateChanged()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_actionOpen_triggered()), Qt::UniqueConnection);

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

void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"Error","File not opened.");
        currentFile = "";
        return;
    }
    QTextStream stream(&file);
    // sauvegarde du contenu du stream
    currentStream = stream.readAll();
    // reconstruction du stream
    QTextStream stream2(&currentStream);
    ui->widget->loadFile(stream2);
    file.close();
}

void MainWindow::on_button_crust_released()
{
    if(currentFile == "") {
        QMessageBox::warning(this,"Impossible","No file to apply crust.");
        return;
    }
    QTextStream stream(&currentStream);
    ui->widget->applyCrust(stream);
}
