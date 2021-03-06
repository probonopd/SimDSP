/*
 * SimDSP Audio.
 *
 * Copyright (c) 2017 lmcapacho
 *
 * This file is part of SimDSP.
 *
 * SimDSP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimDSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimDSP.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "sdaudio.h"

SDAudio::SDAudio(QObject *parent) :
    QObject(parent)
    , audioInputDevice(QAudioDeviceInfo::defaultInputDevice())
    , audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
    , audioInput(0)
    , audioOutput(0)
    , inputData(0)
{
    audioFormat.setSampleSize(16);
    audioFormat.setSampleRate(8000);
    audioFormat.setChannelCount(1);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setSampleType(QAudioFormat::SignedInt);
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
}

SDAudio::~SDAudio()
{
    delete buffer;
    audioInput->stop();
    audioOutput->stop();
}

void SDAudio::initSoundCard(int bSize, double fs)
{
    bufferSize = bSize;
    buffer =  new QByteArray(bufferSize, 0);

    setSampleRate(fs);

    QAudioDeviceInfo infoInput(QAudioDeviceInfo::defaultInputDevice());
    if (!infoInput.isFormatSupported(audioFormat))
    {
        qInfo() << "Default format not supported - trying to use nearest";
        audioFormat = infoInput.nearestFormat(audioFormat);
    }

    QAudioDeviceInfo infoOutput(QAudioDeviceInfo::defaultOutputDevice());

    if (!infoOutput.isFormatSupported(audioFormat))
    {
        qInfo() << "Default format not supported - trying to use nearest";
        audioFormat = infoOutput.nearestFormat(audioFormat);
    }
    createAudioInput();
    createAudioOutput();
}

void SDAudio::createAudioOutput()
{
    audioOutput = new QAudioOutput(audioOutputDevice, audioFormat, this);
    audioOutput->setBufferSize(bufferSize);
}

void SDAudio::createAudioInput()
{
    audioInput = new QAudioInput(audioInputDevice, audioFormat, this);
}

void SDAudio::record()
{
    if(!inputData){
        inputData = audioInput->start();
    }
    inputData->reset();
    connect(inputData, &QIODevice::readyRead, this, &SDAudio::readMore);
}

void SDAudio::play(short *outBuffer)
{
    if(!outputData){
        outputData = audioOutput->start();
    }
    qint64 dataRemaining = bufferSize;
    char* charBuffer = reinterpret_cast<char*>(outBuffer);

    while (dataRemaining) {
        qint64 bytesWritten = outputData->write(charBuffer, dataRemaining);
        dataRemaining -= bytesWritten;
        charBuffer += bytesWritten;
    }
    emit playFinish();
}

void SDAudio::readMore()
{
    if(!audioInput )
        return;

    qint64 len = audioInput->bytesReady();    

    if(len < bufferSize)
        return;

    if(len > bufferSize)
        len = bufferSize;

    qint64 l = inputData->read(buffer->data(), len);
    if(l > 0){
        disconnect(inputData, 0, this, 0);
        emit recordFinish((short*)buffer->data()) ;
    }
}

void SDAudio::setSampleRate(int sampleRate)
{
    audioFormat.setSampleRate(sampleRate);
}

void SDAudio::setSampleSize(int sampleSize)
{
    audioFormat.setSampleSize(sampleSize);
}

void SDAudio::setChannelCount(int channelCount)
{
    audioFormat.setChannelCount(channelCount);
}
