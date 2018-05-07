#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QtNetwork>
#include <QList>
#include <QTime>

#define MAX_CHUNK 16

class Download : public QObject
{
  Q_OBJECT
  public:
      Q_PROPERTY(double speed MEMBER m_speed NOTIFY updateSpeed)
      Q_PROPERTY(QString url MEMBER m_url CONSTANT)
      Q_PROPERTY(quint64 fileSize MEMBER m_fileSize NOTIFY headerReceived)
      Q_PROPERTY(quint64 received MEMBER m_received NOTIFY updateReceived)
      Q_PROPERTY(QString name MEMBER m_name NOTIFY headerReceived)
      Q_PROPERTY(int errorcode MEMBER m_errorcode NOTIFY errorReceived)
      Q_INVOKABLE void abort();
      Q_INVOKABLE void restart();
      explicit Download(QString url, QObject *parent = nullptr);
      QNetworkReply *currentDownload = nullptr;
      void beginDownload();

  signals:
      void updateSpeed();
      void updateReceived();
      void headerReceived();
      void errorReceived();
      void abortDownload();

  private slots:
      void downloadFinished(QNetworkReply*);
      void receivedHeader();
      void errorReceived(QNetworkReply::NetworkError code);
      void downloadReadyRead();
  private:
      QString m_url;
      QVector<QNetworkReply *> m_chunks;
      QNetworkAccessManager m_manager;
      QByteArray m_tempStorage[MAX_CHUNK];
      QFile* m_files[MAX_CHUNK];
      quint64 m_chunkSizes[MAX_CHUNK];
      QTime m_time;

      quint64 m_chunkReceived = 0;
      quint64 m_chunkSize = 0;
      quint64 m_fileSize = 0;
      quint64 m_received = 0;
      double m_speed = 0;

      QString m_name = "Loading...";

      int m_errorcode = 0;

      QString saveFileName(QString path);
      void cleanup();
      void reset();
};

#endif // DOWNLOAD_H
