#include "windows/about_window/AboutWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QApplication>
#include <QDialogButtonBox>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>
#include <QHBoxLayout>

AboutWindow::AboutWindow(MainWindowState* state, QWidget* parent) : QWidget(parent), windowState(state) {
    setupUI();
}

void AboutWindow::setupUI() {
    setFixedSize(400, 300);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Application icon
    QLabel* iconLabel = new QLabel(this);
    QIcon appIcon = QApplication::windowIcon();
    if (!appIcon.isNull()) {
        iconLabel->setPixmap(appIcon.pixmap(64, 64));
        iconLabel->setAlignment(Qt::AlignCenter);
    }

    // Application name
    QLabel* nameLabel = new QLabel("Work Day Tracker", this);
    nameLabel->setAlignment(Qt::AlignCenter);
    QFont nameFont = nameLabel->font();
    nameFont.setPointSize(16);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);

    // Version information
    QLabel* versionLabel = new QLabel("Version 1.0.0", this);
    versionLabel->setAlignment(Qt::AlignCenter);

    // Description
    QLabel* descriptionLabel = new QLabel("A simple application to track your work day activities.", this);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);

    // Links
    QHBoxLayout* linksLayout = new QHBoxLayout();

    QPushButton* websiteButton = new QPushButton("Visit Website", this);
    connect(websiteButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://barmaglote-extra.github.io/work-time-tracker/"));
    });

    QPushButton* coffeeButton = new QPushButton("Buy Me Coffee", this);
    connect(coffeeButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://buymeacoffee.com/barmaglote"));
    });

    linksLayout->addWidget(websiteButton);
    linksLayout->addWidget(coffeeButton);

    // OK button
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QWidget::close);

    // Add widgets to layout
    mainLayout->addWidget(iconLabel);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(versionLabel);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addLayout(linksLayout);
    mainLayout->addWidget(buttonBox);
}