#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QtNetwork>
#include <QRegularExpression>
#include "download.h"

class DownloadManager : public QObject
{
  Q_OBJECT
  public:
    Q_PROPERTY(QList<QObject*> downloads READ downloads NOTIFY downloadsChanged)
    Q_INVOKABLE void addToQueue(QString url);
    explicit DownloadManager(QObject *parent = nullptr);
    QNetworkReply *currentDownload = nullptr;

    QList<QObject*> downloads()
    {
      return m_downloads;
    }

    double speed()
    {
      return m_speed;
    }

    quint64 bytesReceived()
    {
      return m_bytesReceived;
    }

  signals:
    void downloadsChanged();

  private slots:
    void downloadFinished();

  private:
    QList<QObject*> m_downloads;
    double m_speed;
    quint64 m_bytesReceived;
};

#endif // DOWNLOADMANAGER_H
