#include "download.h"

Download::Download(QString url, QObject *parent) : QObject(parent)
{
  m_url = url;
  connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
          SLOT(downloadFinished(QNetworkReply*)));
  for (int i = 0 ; i < MAX_CHUNK ; i++)
  {
    m_files[i] = NULL;
  }
}

QString Download::saveFileName(QString path)
{
  QString basename = QFileInfo(path).fileName();

  if (basename.isEmpty())
    basename = "download";

  return basename;
}

void Download::reset()
{
  m_chunkReceived = 0;
  m_chunkSize = 0;
  m_fileSize = 0;
  m_received = 0;
  m_speed = 0;
  m_name = "Loading...";
  m_tempStorage->clear();
  m_chunks.clear();
  m_errorcode = 0;
  for (int i = 0 ; i < MAX_CHUNK ; i++)
  {
    m_chunkSizes[i] = 0;
  }
  emit errorReceived();
}

void Download::cleanup()
{
  m_chunkReceived = 0;
  m_chunkSize = 0;
  m_tempStorage->clear();
  m_chunks.clear();
  for (int i = 0 ; i < MAX_CHUNK ; i++)
  {
    m_chunkSizes[i] = 0;
    if (m_files[i] != NULL)
    {
      delete m_files[i];
    }
  }
}

void Download::restart()
{
  beginDownload();
}

void Download::abort()
{
  emit abortDownload();
}

void Download::beginDownload()
{
  reset();
  QNetworkRequest request(m_url);
  currentDownload = m_manager.head(request);
  connect(
      currentDownload,
      SIGNAL(finished()),
      SLOT(receivedHeader())
  );
  connect(
      currentDownload,
      SIGNAL(error(QNetworkReply::NetworkError)),
      SLOT(errorReceived(QNetworkReply::NetworkError)));
  connect(
      this,
      SIGNAL(abortDownload()),
      currentDownload,
      SLOT(abort())
  );
}

void Download::errorReceived(QNetworkReply::NetworkError code)
{
  qDebug() << "Error: " << code ;
  m_errorcode = code;
  emit errorReceived();
}

void Download::receivedHeader()
{
  m_fileSize = currentDownload->rawHeader("Content-Length").toInt();
  QString acceptRanges = QString::fromStdString(currentDownload->rawHeader("Accept-Ranges").toStdString());
  m_chunkSize = m_fileSize/16;
  if (acceptRanges == "bytes")
  {
    m_name = saveFileName(m_url);
    for (int i = 0 ; i < MAX_CHUNK; i++)
    {
      QNetworkRequest request(m_url);
      quint32 beginRange = i * m_chunkSize;
      quint32 endRange = beginRange + m_chunkSize;
      if (i != 0)
      {
        beginRange++;
      }
      if (i == (MAX_CHUNK - 1))
      {
        endRange = beginRange + (m_fileSize - 1);
      }
      QString range("bytes=");
      range = range + QString::number(beginRange) + "-" + QString::number(endRange);
      request.setRawHeader("Range", range.toUtf8());
      QNetworkReply *reply = m_manager.get(request);
      connect(
          this,
          SIGNAL(abortDownload()),
          reply,
          SLOT(abort())
      );
      connect(
          reply,
          SIGNAL(readyRead()),
          SLOT(downloadReadyRead())
      );
      connect(
          reply,
          SIGNAL(error(QNetworkReply::NetworkError)),
          SLOT(errorReceived(QNetworkReply::NetworkError)));
      QString filename(m_name + "_" + QString::number(i));
      m_files[i] = new QFile(filename);
      if (!m_files[i]->open(QIODevice::ReadWrite))
      {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(m_name),
                qPrintable(m_files[i]->errorString()));
        return;
      }
      m_chunks.append(reply);
      QTime downloadTime;
      downloadTime.start();
      m_time = downloadTime;
      emit headerReceived();
    }
  }
}

void Download::downloadFinished(QNetworkReply* reply)
{
  m_chunkReceived++;
  if (m_chunkReceived == (MAX_CHUNK + 1))
  {
    QFile finalFile(m_name);
    if (!finalFile.open(QIODevice::WriteOnly))
    {
      fprintf(stderr, "Could not open %s for writing: %s\n",
              qPrintable(m_name),
              qPrintable(finalFile.errorString()));
      return;
    }

    for (int i = 0 ; i < MAX_CHUNK ; i++)
    {
      m_files[i]->reset();
      finalFile.write(m_files[i]->readAll());
      m_files[i]->remove();
    }
    finalFile.close();
    cleanup();
    qDebug() << "All done";
  }
}

void Download::downloadReadyRead()
{
  m_received = 0;
  for (int i = 0 ; i < MAX_CHUNK ; i++)
  {
    m_chunkSizes[i] += m_chunks.at(i)->bytesAvailable();
    m_received += m_chunkSizes[i];
    m_files[i]->write(m_chunks.at(i)->readAll());
  }
  double speed = m_received * 1000.0 / m_time.elapsed();
  speed /= 1024*1024;
  m_speed = speed;
  if (m_received != 0)
  {
    emit updateSpeed();
    emit updateReceived();
  }
}
