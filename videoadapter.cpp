#include "videoadapter.h"
#include <QtMultimedia>

 VideoAdapter::VideoAdapter(QObject *parent)
     : QAbstractVideoSurface(parent),
       m_outputSurface(0),
       m_player(static_cast<QMediaPlayer*>(parent)),
       m_bufferTotal(0),
       m_lastPos(0),
       m_duration(0),
       m_returnPos(0)
 {
     connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));
     connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));

     connect(m_player, SIGNAL(durationChanged(qint64)), this, SIGNAL(durationChanged(qint64)));
     connect(m_player, SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64)));
     connect(m_player, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
 }

 QList<QVideoFrame::PixelFormat> VideoAdapter::supportedPixelFormats(
         QAbstractVideoBuffer::HandleType handleType) const
 {
     if (handleType == QAbstractVideoBuffer::NoHandle) {
         return QList<QVideoFrame::PixelFormat>()
                 << QVideoFrame::Format_RGB32
                 << QVideoFrame::Format_ARGB32
                 << QVideoFrame::Format_ARGB32_Premultiplied
                 << QVideoFrame::Format_RGB565
                 << QVideoFrame::Format_RGB555;
     } else {
         return QList<QVideoFrame::PixelFormat>();
     }
 }

 bool VideoAdapter::isFormatSupported(
         const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const
 {
     Q_UNUSED(similar);

     const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     return imageFormat != QImage::Format_Invalid
             && !size.isEmpty()
             && format.handleType() == QAbstractVideoBuffer::NoHandle;
 }

 void VideoAdapter::stop()
 {
     if (m_outputSurface)
        m_outputSurface->stop();
 }

 bool VideoAdapter::present(const QVideoFrame &frame)
 {
//     qDebug() << frame;
     if(m_outputSurface && !m_outputSurface->isActive()) {
         m_outputSurface->start(surfaceFormat());
         m_player->setPosition(0);
     }
     m_outputSurface->present(frame);

     m_lastPos = m_player->position();
     m_duration = m_player->duration();
     return true;
 }

 void VideoAdapter::onStateChanged(QMediaPlayer::State state)
 {
     switch (state) {
     case QMediaPlayer::PlayingState:
         m_playbackState = 1;
         break;
     case QMediaPlayer::PausedState:
         m_playbackState = 2;
         break;
     default:
         m_playbackState = 0;
     }

     emit playbackStateChanged(m_playbackState);
 }

 void VideoAdapter::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
 {
     if(status == QMediaPlayer::EndOfMedia) {
         qDebug() << "End: " << m_lastPos << ":" << m_duration;
        if(m_duration - m_lastPos < 500) {
            m_playbackState = 3;
            emit playbackStateChanged(m_playbackState);
        } else {
            qDebug() << "RESET";
            m_player->play();
            m_returnPos = m_lastPos;
        }
     }

     if(status == QMediaPlayer::BufferedMedia && m_returnPos > 0) {
         m_player->setPosition(m_returnPos);
         m_returnPos = 0;
     }

     if(status == QMediaPlayer::InvalidMedia) {
         m_returnPos = m_lastPos;
         emit resetMedia();
     }
     qDebug() << status;
 }

void VideoAdapter::bufferProgress(qint64 current, qint64 total)
{
    m_bufferCurrent = current;
    if(total != m_bufferTotal) {
        m_bufferTotal = total;
        emit bufferTotalChanged(m_bufferTotal);
    }
    emit bufferCurrentChanged(m_bufferCurrent);
}
