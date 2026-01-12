#include "mainwindow.h"
#include "tetrisgame.h"
#include "tetrisboard.h"
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_board(nullptr)
    , m_game(nullptr)
{
    // 创建游戏对象
    m_game = new TetrisGame(this);
    
    // 创建游戏板
    m_board = new TetrisBoard(m_game, this);
    
    setupUI();
    createMenuBar();
    createStatusBar();
    connectSignals();
    
    setWindowTitle("俄罗斯方块");
    resize(600, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    m_mainLayout = new QHBoxLayout(centralWidget);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);

    // 创建左侧控制面板
    m_infoLayout = new QVBoxLayout();
    m_infoLayout->setSpacing(15);

    // 分数标签
    m_scoreLabel = new QLabel("分数:", this);
    m_scoreLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: white; }");
    m_scoreValue = new QLabel("0", this);
    m_scoreValue->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #00ff00; }");
    m_scoreValue->setAlignment(Qt::AlignCenter);

    // 等级标签
    m_levelLabel = new QLabel("等级:", this);
    m_levelLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: white; }");
    m_levelValue = new QLabel("1", this);
    m_levelValue->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #ffff00; }");
    m_levelValue->setAlignment(Qt::AlignCenter);

    // 行数标签
    m_linesLabel = new QLabel("行数:", this);
    m_linesLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: white; }");
    m_linesValue = new QLabel("0", this);
    m_linesValue->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #00ffff; }");
    m_linesValue->setAlignment(Qt::AlignCenter);

    // 添加到信息布局
    m_infoLayout->addWidget(m_scoreLabel);
    m_infoLayout->addWidget(m_scoreValue);
    m_infoLayout->addSpacing(20);
    m_infoLayout->addWidget(m_levelLabel);
    m_infoLayout->addWidget(m_levelValue);
    m_infoLayout->addSpacing(20);
    m_infoLayout->addWidget(m_linesLabel);
    m_infoLayout->addWidget(m_linesValue);
    m_infoLayout->addSpacing(30);

    // 创建按钮
    m_startButton = new QPushButton("开始游戏", this);
    m_startButton->setStyleSheet("QPushButton { font-size: 14px; padding: 12px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");
    m_startButton->setMinimumWidth(120);

    m_pauseButton = new QPushButton("暂停", this);
    m_pauseButton->setStyleSheet("QPushButton { font-size: 14px; padding: 12px; background-color: #ff9800; color: white; border: none; border-radius: 5px; } QPushButton:hover { background-color: #e68a00; }");
    m_pauseButton->setEnabled(false);
    m_pauseButton->setMinimumWidth(120);

    m_resetButton = new QPushButton("重置", this);
    m_resetButton->setStyleSheet("QPushButton { font-size: 14px; padding: 12px; background-color: #f44336; color: white; border: none; border-radius: 5px; } QPushButton:hover { background-color: #da190b; }");
    m_resetButton->setMinimumWidth(120);

    m_infoLayout->addWidget(m_startButton);
    m_infoLayout->addSpacing(10);
    m_infoLayout->addWidget(m_pauseButton);
    m_infoLayout->addSpacing(10);
    m_infoLayout->addWidget(m_resetButton);
    m_infoLayout->addStretch();

    // 创建游戏布局（游戏板居中）
    m_gameLayout = new QHBoxLayout();
    m_gameLayout->addStretch();
    m_gameLayout->addWidget(m_board);
    m_gameLayout->addStretch();

    // 添加到主布局：左侧控制面板 + 中间游戏板
    m_mainLayout->addLayout(m_infoLayout);
    m_mainLayout->addLayout(m_gameLayout, 1);

    // 设置背景色
    centralWidget->setStyleSheet("QWidget { background-color: #1a1a2e; }");
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // 游戏菜单
    QMenu *gameMenu = menuBar->addMenu("游戏(&G)");
    
    QAction *startAction = gameMenu->addAction("开始(&S)");
    startAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(startAction, &QAction::triggered, this, &MainWindow::startGame);
    
    QAction *pauseAction = gameMenu->addAction("暂停(&P)");
    pauseAction->setShortcut(QKeySequence("Ctrl+P"));
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pauseGame);
    
    gameMenu->addSeparator();
    
    QAction *resetAction = gameMenu->addAction("重置(&R)");
    resetAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetGame);
    
    gameMenu->addSeparator();
    
    QAction *exitAction = gameMenu->addAction("退出(&X)");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // 帮助菜单
    QMenu *helpMenu = menuBar->addMenu("帮助(&H)");
    
    QAction *aboutAction = helpMenu->addAction("关于(&A)");
    connect(aboutAction, &QAction::triggered, [this]() {
        QString aboutText = QString::fromUtf8(
            "<h2>俄罗斯方块 v1.0</h2>"

            "<h3>游戏控制</h3>"
            "<table border='1' cellpadding='4' cellspacing='0' style='width:100%'>"
            "<tr><td colspan='2'><b>方向键 / Vim</b></td><td><b>功能</b></td></tr>"
            "<tr><td>← / h</td><td>→ / l</td><td>左右移动</td></tr>"
            "<tr><td>↓ / j</td><td>↑ / k</td><td>加速下落 / 旋转</td></tr>"
            "<tr><td colspan='2'><b>空格</b></td><td>直接落地</td></tr>"
            "<tr><td colspan='3'><hr></td></tr>"
            "<tr><td>Ctrl+S</td><td>Ctrl+P</td><td>开始 / 暂停</td></tr>"
            "<tr><td>Ctrl+R</td><td>Ctrl+Q</td><td>重置 / 退出</td></tr>"
            "</table>"

            "<h3>得分规则</h3>"
            "<ul>"
            "<li>消除1行: 100 × 等级</li>"
            "<li>消除2行: 300 × 等级</li>"
            "<li>消除3行: 500 × 等级</li>"
            "<li>消除4行: 800 × 等级</li>"
            "<li>每消除10行升一级</li>"
            "</ul>"

            "<p style='margin:5px 0;'><i>祝您游戏愉快！</i></p>"
        );
        QMessageBox::about(this, "关于俄罗斯方块", aboutText);
    });
}

void MainWindow::createStatusBar()
{
    QStatusBar *statusBar = this->statusBar();
    statusBar->showMessage("准备开始游戏");
}

void MainWindow::connectSignals()
{
    // 连接游戏信号
    connect(m_game, &TetrisGame::boardChanged, m_board, QOverload<>::of(&TetrisBoard::update));
    connect(m_game, &TetrisGame::pieceChanged, m_board, QOverload<>::of(&TetrisBoard::update));
    connect(m_game, &TetrisGame::scoreChanged, this, &MainWindow::updateScore);
    connect(m_game, &TetrisGame::levelChanged, this, &MainWindow::updateLevel);
    connect(m_game, &TetrisGame::linesChanged, this, &MainWindow::updateLines);
    connect(m_game, &TetrisGame::gameOverSignal, this, &MainWindow::handleGameOver);
    
    // 连接按钮信号
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::pauseGame);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::resetGame);
}

void MainWindow::startGame()
{
    if (m_game->isPaused()) {
        m_game->resume();
        m_pauseButton->setText("暂停");
        m_pauseButton->setEnabled(true);
        statusBar()->showMessage("游戏继续");
    } else {
        m_game->start();
        m_startButton->setText("重新开始");
        m_pauseButton->setEnabled(true);
        statusBar()->showMessage("游戏进行中");
    }
    
    m_board->setFocus();
}

void MainWindow::pauseGame()
{
    if (m_game->isPaused()) {
        m_game->resume();
        m_pauseButton->setText("暂停");
        statusBar()->showMessage("游戏继续");
    } else {
        m_game->pause();
        m_pauseButton->setText("继续");
        statusBar()->showMessage("游戏暂停");
    }
    
    m_board->setFocus();
}

void MainWindow::resetGame()
{
    m_game->reset();
    m_startButton->setText("开始游戏");
    m_pauseButton->setText("暂停");
    m_pauseButton->setEnabled(false);
    statusBar()->showMessage("游戏已重置");
    
    m_board->setFocus();
}

void MainWindow::updateScore(int score)
{
    m_scoreValue->setText(QString::number(score));
}

void MainWindow::updateLevel(int level)
{
    m_levelValue->setText(QString::number(level));
}

void MainWindow::updateLines(int lines)
{
    m_linesValue->setText(QString::number(lines));
}

void MainWindow::handleGameOver()
{
    m_pauseButton->setEnabled(false);
    statusBar()->showMessage("游戏结束");
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "游戏结束",
        QString("游戏结束！\n最终分数: %1\n\n是否重新开始？").arg(m_game->getScore()),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        resetGame();
        startGame();
    }
}