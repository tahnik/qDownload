#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
}

void DownloadManager::addToQueue(QString url)
{
  QRegularExpression re("[\\S]+");
  QRegularExpressionMatch match = re.match(url);
  bool hasMatch = match.hasMatch();
  if (hasMatch)
  {
    Download *file = new Download(url);
    file->beginDownload();
    m_downloads.append(file);
    emit downloadsChanged();
  }
  else
  {
    qDebug() << "Wrong URL";
  }
}

void DownloadManager::downloadFinished()
{
  qDebug() << "Done Lads";
}
