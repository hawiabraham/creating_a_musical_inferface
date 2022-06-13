#include "mainwindow.h"

#include <QApplication>
#include <rtaudio/RtAudio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16


double streamTimePrintIncrement = 1.0; // seconds
double streamTimePrintTime = 1.0; // seconds

int inout( void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

  if ( streamTime >= streamTimePrintTime ) {
    std::cout << "streamTime = " << streamTime << std::endl;
    streamTimePrintTime += streamTimePrintIncrement;
  }

  unsigned int *bytes = (unsigned int *) data;
  memcpy( outputBuffer, inputBuffer, *bytes );
  return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    unsigned int channels = 1;
    unsigned int  fs = 48000;
    unsigned int bufferBytes = 256;
    unsigned int oDevice = 0, iDevice = 0, iOffset = 0, oOffset = 0;

    RtAudio adac;
    if ( adac.getDeviceCount() < 1 ) {
      std::cout << "\nNo audio devices found!\n";
      exit( 1 );
    }

    // Let RtAudio print messages to stderr.
    adac.showWarnings( true );

    // Set the same number of channels for both input and output.
    unsigned int bufferFrames = 512;
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = iDevice;
    iParams.nChannels = channels;
    iParams.firstChannel = iOffset;
    oParams.deviceId = oDevice;
    oParams.nChannels = channels;
    oParams.firstChannel = oOffset;

    if ( iDevice == 0 )
      iParams.deviceId = adac.getDefaultInputDevice();
    if ( oDevice == 0 )
      oParams.deviceId = adac.getDefaultOutputDevice();

    RtAudio::StreamOptions options;

    bufferBytes = bufferFrames * channels * sizeof( MY_TYPE );
    adac.openStream( &oParams, &iParams, FORMAT, fs,
                     &bufferFrames, &inout, (void *)&bufferBytes,
                     &options );
    if ( adac.isStreamOpen() == false ) goto cleanup;

    std::cout << "\nStream latency = " << adac.getStreamLatency() << " frames" << std::endl;

    adac.startStream();

    char input;
    std::cout << "\nRunning ... press <enter> to quit (buffer frames = " << bufferFrames << ").\n";
    std::cin.get(input);

cleanup:
 if ( adac.isStreamOpen() ) adac.closeStream();

    return a.exec();
}
