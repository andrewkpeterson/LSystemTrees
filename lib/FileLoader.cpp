#include "FileLoader.h"
#include <QFile>
#include <QString>
#include <QTextStream>

std::string FileLoader::loadResourceFileToString(const std::string &resourcePath)
{
    QString vertFilePath = QString::fromStdString(resourcePath);
    QFile vertFile(vertFilePath);
    if (vertFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream vertStream(&vertFile);
        QString contents = vertStream.readAll();
        return contents.toStdString();
    }
    throw CS123::IOException("Could not open file: " + resourcePath);
}
