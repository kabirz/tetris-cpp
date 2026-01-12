#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class TetrisGame;
class TetrisBoard;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startGame();
    void pauseGame();
    void resetGame();
    void updateScore(int score);
    void updateLevel(int level);
    void updateLines(int lines);
    void handleGameOver();

private:
    void setupUI();
    void createMenuBar();
    void createStatusBar();
    void connectSignals();

    // UI组件
    TetrisBoard *m_board;
    TetrisGame *m_game;

    QLabel *m_scoreLabel;
    QLabel *m_scoreValue;
    QLabel *m_levelLabel;
    QLabel *m_levelValue;
    QLabel *m_linesLabel;
    QLabel *m_linesValue;

    QPushButton *m_startButton;
    QPushButton *m_pauseButton;
    QPushButton *m_resetButton;

    // 布局
    QHBoxLayout *m_mainLayout;
    QHBoxLayout *m_gameLayout;
    QVBoxLayout *m_infoLayout;
};

#endif // MAINWINDOW_H