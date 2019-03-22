#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
public:
    explicit Backend(QObject *parent = nullptr);

    QString data();
    void setData(const QString &data);
signals:
    void dataChanged();

public slots:

private:
    QString m_data;
};

#endif // BACKEND_H
