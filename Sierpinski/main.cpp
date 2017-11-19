#include "mainwindow.h"
#include "crust.h"
#include "iterators.h"
#include <iostream>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    /*
    //Triangulation t("/home/ad/Documents/M2/GAM/carre.off");
    Triangulation t("/home/ad/Documents/M2/GAM/test.pts");

    VertexIterator v_it;
    FaceCirculator f_c;

    // parcours des sommets
    for(v_it = t.beginVertex();v_it != t.endVertex();++v_it) {
        // recup face liee au sommet
        FaceCirculator f_c_begin = t.getIncidentFace(*v_it);
        // init compteur de faces liees au sommet
        unsigned int cpt = 1;
        // parcours des faces
        for(f_c = f_c_begin, ++f_c; f_c != f_c_begin;++f_c) {
            // incrementation du compteur de faces
            cpt++;
        }
        // affichage nombre de faces liees au sommet
        std::cout << "valence du sommet " << cpt << std::endl;
    }

    */


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

/*    QString fileName("/home/ad/Documents/M2/GAM/test_crust/line3.pts");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        return -1;
    }
    QTextStream stream(&file);
    Crust crust;
    crust = Crust(stream, false);

    QString fileName2("/home/ad/Documents/M2/GAM/test_crust/heart.off");
    QFile file2(fileName2);
    if (!file2.open(QIODevice::WriteOnly | QFile::Text)) {
        return -2;
    }
    QTextStream stream2(&file2);
    crust.saveAs(stream2);

    file.close();
    file2.flush();
    file2.close();

    return 0;*/
}


