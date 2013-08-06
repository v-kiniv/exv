#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QtCore/QRect>
#include <QtGui/QImage>
#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QVideoFrame>
#include <QMediaPlayer>

class VideoAdapter : public QAbstractVideoSurface
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getOutputSurface WRITE setOutputSurface)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(int playbackState READ playbackState NOTIFY playbackStateChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(qint64 bufferCurrent READ bufferCurrent NOTIFY bufferCurrentChanged)
    Q_PROPERTY(qint64 bufferTotal READ bufferTotal NOTIFY bufferTotalChanged)
public:
     VideoAdapter(QObject *parent = 0);

     QList<QVideoFrame::PixelFormat> supportedPixelFormats(
             QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
     bool isFormatSupported(const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const;
     void stop();
     bool present(const QVideoFrame &frame);

     QAbstractVideoSurface* getOutputSurface() { return m_outputSurface; }
     void setOutputSurface(QAbstractVideoSurface *vs) { m_outputSurface = vs; }
     int playbackState() { return m_playbackState; }

     qint64 duration() { return m_player->duration(); }
     qint64 position() { return m_player->position(); }
     void setPosition(qint64 &pos) { m_player->setPosition(pos); }

     int volume() { m_player->volume(); }
     void setVolume(int &volume) { m_player->setVolume(volume); }

     qint64 bufferCurrent() { return m_bufferCurrent; }
     qint64 bufferTotal() { return m_bufferTotal; }

 private:
     QAbstractVideoSurface* m_outputSurface;
     QMediaPlayer *m_player;
     int m_playbackState;
     qint64 m_bufferCurrent;
     qint64 m_bufferTotal;
     qint64 m_lastPos;
     qint64 m_duration;
     qint64 m_returnPos;

private slots:
     void onStateChanged(QMediaPlayer::State);
     void onMediaStatusChanged(QMediaPlayer::MediaStatus);

public slots:
     void bufferProgress(qint64 current, qint64 total);
     void seek(qint64 pos) {
//         while(m_player->position() != pos)
         m_player->setPosition(pos);
         emit positionChanged(pos);
     }
     void play() { m_player->play(); }
     void pause() { m_player->pause(); }

signals:
     void resetMedia();
     void frameChanged(QVideoFrame frame);
     void playbackStateChanged(int);
     void durationChanged(qint64);
     void positionChanged(qint64);
     void volumeChanged(int);
     void bufferCurrentChanged(qint64);
     void bufferTotalChanged(qint64);
    
};

#endif // VIDEOSURFACE_H
