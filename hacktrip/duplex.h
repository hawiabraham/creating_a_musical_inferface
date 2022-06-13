#ifndef DUPLEX_H
#define DUPLEX_H

#include <QThread>
#include <QObject>
#include <rtaudio/RtAudio.h>

class Duplex : public QThread
{
public:
    explicit Duplex(QObject *parent = nullptr);
private:

    unsigned int channels;
    unsigned int  fs;
    unsigned int bufferBytes;
    unsigned int oDevice, iDevice,
    iOffset, oOffset;
    RtAudio* adac;
    RtAudio::StreamParameters iParams, oParams;

    virtual void run();


};

#endif // DUPLEX_H
