#ifndef ConverterD_H
#define ConverterD_H

#include <QObject>
#include <string>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = 0) {}
    Converter(const std::string& openPath, const std::string& savePath, const std::string& saveName, const int& colorType);

private:
    std::string openPath;
    std::string savePath;
    std::string saveName;
    int colorType;

signals:
    void conversionStart();
    void conversionEnd();
    void conversionProgress(int value);
    void conversionSuccess();
    void conversionFailed();
    void finished();
    void error(QString err);

public slots:
    void run();
};

#endif // ConverterD_H
