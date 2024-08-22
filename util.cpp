#include"util.h"

void play(QString filename){
    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile(filename));
    audioOutput->setVolume(50);
    player->play();
}
