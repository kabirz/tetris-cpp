#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QVector>
#include <QPoint>
#include <QColor>
#include <QTimer>
#include <QObject>

// 方块形状定义
enum class Tetromino {
    I, O, T, S, Z, J, L
};

// 方块旋转状态
enum class Rotation {
    North, East, South, West
};

class TetrisGame : public QObject
{
    Q_OBJECT

public:
    explicit TetrisGame(QObject *parent = nullptr);
    ~TetrisGame();

    // 游戏控制
    void start();
    void pause();
    void resume();
    void reset();

    // 方块移动
    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();
    void hardDrop();

    // 游戏状态查询
    bool isGameOver() const;
    bool isPaused() const;
    int getScore() const;
    int getLevel() const;
    int getLines() const;

    // 获取游戏板数据
    const QVector<QVector<int>>& getBoard() const;
    QVector<QPoint> getCurrentPiece() const;
    QColor getCurrentPieceColor() const;
    QVector<QPoint> getNextPiece() const;
    QColor getNextPieceColor() const;
    QPoint getCurrentPos() const;
    QPoint getShadowPos() const;

signals:
    void boardChanged();
    void scoreChanged(int score);
    void levelChanged(int level);
    void linesChanged(int lines);
    void gameOverSignal();
    void pieceChanged();

private slots:
    void gameLoop();

private:
    // 游戏板
    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;
    QVector<QVector<int>> m_board;

    // 当前方块
    Tetromino m_currentTetromino;
    Rotation m_currentRotation;
    QPoint m_currentPos;
    QColor m_currentColor;

    // 下一个方块
    Tetromino m_nextTetromino;
    QColor m_nextColor;

    // 游戏状态
    bool m_gameOver;
    bool m_paused;
    int m_score;
    int m_level;
    int m_lines;

    // 游戏循环
    QTimer *m_gameTimer;
    int m_dropInterval;

    // 方块定义
    QVector<QPoint> getTetrominoShape(Tetromino type, Rotation rotation) const;
    QColor getTetrominoColor(Tetromino type) const;
    Tetromino getRandomTetromino() const;

    // 碰撞检测
    bool isValidPosition(const QVector<QPoint>& piece, const QPoint& pos) const;
    bool checkCollision(const QVector<QPoint>& piece, const QPoint& pos) const;

    // 方块操作
    void spawnPiece();
    void lockPiece();
    void clearLines();
    void updateLevel();

    // 辅助函数
    void updateBoard();
};

#endif // TETRISGAME_H