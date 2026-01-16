#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QPixmap>
#include <QSlider>
#include <QDesktopServices>
#include <QIcon>
#include <QDir>
#include <QFileInfo>

class RadioWindow : public QMainWindow {
    Q_OBJECT

public:
    RadioWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Fur Radio Client v1.0 (C++)");
        setFixedSize(400, 500);

        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(20, 20, 20, 20);

        iconLabel = new QLabel(this);
        QString iconPath;
        QString appDir = QApplication::applicationDirPath();
        QString resourcesPath = appDir + "/../Resources/icon.png";
        if (QFileInfo::exists(resourcesPath)) {
            iconPath = QDir(resourcesPath).absolutePath();
        } else if (QFileInfo::exists(appDir + "/icon.png")) {
            iconPath = appDir + "/icon.png";
        } else if (QFileInfo::exists(QDir::current().absoluteFilePath("icon.png"))) {
            iconPath = QDir::current().absoluteFilePath("icon.png");
        } else {
            iconPath = "/Users/shuvee/Desktop/C++ Learn/icon.png";
        }
        QPixmap pixmap(iconPath);
        if (!pixmap.isNull()) {
            pixmap = pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            iconLabel->setPixmap(pixmap);
            QIcon appIcon(iconPath);
            setWindowIcon(appIcon);
            QApplication::setWindowIcon(appIcon);
        }
        iconLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(iconLabel);

        titleLabel = new QLabel("Fur Radio", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel->font();
        titleFont.setPointSize(18);
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);
        mainLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel("Fur-Radio: Твій природний ритм.", this);
        subtitleLabel->setAlignment(Qt::AlignCenter);
        QFont subtitleFont = subtitleLabel->font();
        subtitleFont.setPointSize(10);
        subtitleLabel->setFont(subtitleFont);
        mainLayout->addWidget(subtitleLabel);

        statusLabel = new QLabel("готово", this);
        statusLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(statusLabel);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        
        playButton = new QPushButton("▶️ Підключитись", this);
        stopButton = new QPushButton("⏸️ Пауза", this);
        stopButton->setEnabled(false);

        buttonLayout->addWidget(playButton);
        buttonLayout->addWidget(stopButton);
        mainLayout->addLayout(buttonLayout);

        QHBoxLayout *volumeLayout = new QHBoxLayout();
        QLabel *volumeLabel = new QLabel("🔊", this);
        volumeSlider = new QSlider(Qt::Horizontal, this);
        volumeSlider->setRange(0, 100);
        volumeSlider->setValue(50);
        volumeLayout->addWidget(volumeLabel);
        volumeLayout->addWidget(volumeSlider);
        mainLayout->addLayout(volumeLayout);

        telegramButton = new QPushButton("👀 Telegram провайдера", this);
        mainLayout->addWidget(telegramButton);

        mainLayout->addStretch();

        audioOutput = new QAudioOutput(this);
        audioOutput->setVolume(0.5);
        mediaPlayer = new QMediaPlayer(this);
        mediaPlayer->setAudioOutput(audioOutput);

        connect(playButton, &QPushButton::clicked, this, &RadioWindow::playRadio);
        connect(stopButton, &QPushButton::clicked, this, &RadioWindow::stopRadio);
        connect(volumeSlider, &QSlider::valueChanged, this, &RadioWindow::onVolumeChanged);
        connect(telegramButton, &QPushButton::clicked, this, &RadioWindow::openTelegram);
        connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &RadioWindow::onMediaStatusChanged);
        connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &RadioWindow::onPlaybackStateChanged);
        connect(mediaPlayer, &QMediaPlayer::errorOccurred, this, &RadioWindow::onError);
    }

private slots:
    void playRadio() {
        QUrl url("https://radio.galebrawl.pp.ua/live");
        if (mediaPlayer->source() != url) {
            mediaPlayer->setSource(url);
            statusLabel->setText("Підключаю...");
        }
        mediaPlayer->play();
        playButton->setEnabled(false);
        stopButton->setEnabled(true);
        if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
            statusLabel->setText("Граю");
        }
    }

    void stopRadio() {
        mediaPlayer->stop();
        statusLabel->setText("Пауза");
        playButton->setEnabled(true);
        stopButton->setEnabled(false);
    }

    void onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::BufferedMedia) {
            if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
                statusLabel->setText("Граю");
            }
        } else if (status == QMediaPlayer::BufferingMedia) {
            if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
                statusLabel->setText("Граю");
            }
        } else if (status == QMediaPlayer::LoadingMedia) {
            statusLabel->setText("Завантажую...");
        }
    }

    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::PlayingState) {
            if (mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia || 
                mediaPlayer->mediaStatus() == QMediaPlayer::BufferingMedia) {
                statusLabel->setText("Граю");
            } else {
                statusLabel->setText("Підключаю...");
            }
        }
    }

    void onError(QMediaPlayer::Error error, const QString &errorString) {
        statusLabel->setText("Error: " + errorString);
        playButton->setEnabled(true);
        stopButton->setEnabled(false);
    }

    void onVolumeChanged(int value) {
        qreal volume = value / 100.0;
        audioOutput->setVolume(volume);
    }

    void openTelegram() {
        QDesktopServices::openUrl(QUrl("https://t.me/shuvee_dev"));
    }

private:
    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *statusLabel;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *telegramButton;
    QSlider *volumeSlider;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QString iconPath;
    QString appDir = QApplication::applicationDirPath();
    QString resourcesPath = appDir + "/../Resources/icon.png";
    if (QFileInfo::exists(resourcesPath)) {
        iconPath = QDir(resourcesPath).absolutePath();
    } else if (QFileInfo::exists(appDir + "/icon.png")) {
        iconPath = appDir + "/icon.png";
    } else if (QFileInfo::exists(QDir::current().absoluteFilePath("icon.png"))) {
        iconPath = QDir::current().absoluteFilePath("icon.png");
    } else {
        iconPath = "/Users/shuvee/Desktop/C++ Learn/icon.png";
    }
    if (QFileInfo::exists(iconPath)) {
        QIcon appIcon(iconPath);
        QApplication::setWindowIcon(appIcon);
    }
    
    RadioWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"
