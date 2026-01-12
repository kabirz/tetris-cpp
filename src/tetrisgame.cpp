#include "tetrisgame.h"
#include <QRandomGenerator>
#include <algorithm>

TetrisGame::TetrisGame(QObject *parent)
    : QObject(parent)
    , m_gameOver(false)
    , m_paused(false)
    , m_score(0)
    , m_level(1)
    , m_lines(0)
    , m_dropInterval(1000)
{
    // 初始化游戏板
    m_board.resize(BOARD_HEIGHT);
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        m_board[i].resize(BOARD_WIDTH, 0);
    }

    // 创建游戏定时器
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &TetrisGame::gameLoop);

    // 生成第一个方块
    m_nextTetromino = getRandomTetromino();
    m_nextColor = getTetrominoColor(m_nextTetromino);
}

TetrisGame::~TetrisGame()
{
    if (m_gameTimer) {
        m_gameTimer->stop();
    }
}

void TetrisGame::start()
{
    reset();
    spawnPiece();
    m_gameTimer->start(m_dropInterval);
}

void TetrisGame::pause()
{
    if (!m_gameOver && !m_paused) {
        m_paused = true;
        m_gameTimer->stop();
    }
}

void TetrisGame::resume()
{
    if (!m_gameOver && m_paused) {
        m_paused = false;
        m_gameTimer->start(m_dropInterval);
    }
}

void TetrisGame::reset()
{
    // 清空游戏板
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            m_board[i][j] = 0;
        }
    }

    // 重置游戏状态
    m_gameOver = false;
    m_paused = false;
    m_score = 0;
    m_level = 1;
    m_lines = 0;
    m_dropInterval = 1000;

    m_gameTimer->stop();

    emit scoreChanged(m_score);
    emit levelChanged(m_level);
    emit linesChanged(m_lines);
}

void TetrisGame::moveLeft()
{
    if (m_gameOver || m_paused) return;

    QPoint newPos = m_currentPos;
    newPos.setX(newPos.x() - 1);

    if (isValidPosition(getCurrentPiece(), newPos)) {
        m_currentPos = newPos;
        emit boardChanged();
    }
}

void TetrisGame::moveRight()
{
    if (m_gameOver || m_paused) return;

    QPoint newPos = m_currentPos;
    newPos.setX(newPos.x() + 1);

    if (isValidPosition(getCurrentPiece(), newPos)) {
        m_currentPos = newPos;
        emit boardChanged();
    }
}

void TetrisGame::moveDown()
{
    if (m_gameOver || m_paused) return;

    QPoint newPos = m_currentPos;
    newPos.setY(newPos.y() + 1);

    if (isValidPosition(getCurrentPiece(), newPos)) {
        m_currentPos = newPos;
        emit boardChanged();
    } else {
        lockPiece();
    }
}

void TetrisGame::rotate()
{
    if (m_gameOver || m_paused) return;

    Rotation newRotation = static_cast<Rotation>((static_cast<int>(m_currentRotation) + 1) % 4);
    QVector<QPoint> newShape = getTetrominoShape(m_currentTetromino, newRotation);

    if (isValidPosition(newShape, m_currentPos)) {
        m_currentRotation = newRotation;
        emit boardChanged();
    }
}

void TetrisGame::hardDrop()
{
    if (m_gameOver || m_paused) return;

    while (isValidPosition(getCurrentPiece(), m_currentPos)) {
        m_currentPos.setY(m_currentPos.y() + 1);
    }
    m_currentPos.setY(m_currentPos.y() - 1);
    lockPiece();
}

bool TetrisGame::isGameOver() const
{
    return m_gameOver;
}

bool TetrisGame::isPaused() const
{
    return m_paused;
}

int TetrisGame::getScore() const
{
    return m_score;
}

int TetrisGame::getLevel() const
{
    return m_level;
}

int TetrisGame::getLines() const
{
    return m_lines;
}

const QVector<QVector<int>>& TetrisGame::getBoard() const
{
    return m_board;
}

QVector<QPoint> TetrisGame::getCurrentPiece() const
{
    if (m_gameOver) return QVector<QPoint>();
    return getTetrominoShape(m_currentTetromino, m_currentRotation);
}

QColor TetrisGame::getCurrentPieceColor() const
{
    return m_currentColor;
}

QVector<QPoint> TetrisGame::getNextPiece() const
{
    return getTetrominoShape(m_nextTetromino, Rotation::North);
}

QColor TetrisGame::getNextPieceColor() const
{
    return m_nextColor;
}

QPoint TetrisGame::getCurrentPos() const
{
    return m_currentPos;
}

void TetrisGame::gameLoop()
{
    moveDown();
}

QVector<QPoint> TetrisGame::getTetrominoShape(Tetromino type, Rotation rotation) const
{
    QVector<QPoint> shape;
    
    switch (type) {
        case Tetromino::I:
            shape = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
            break;
        case Tetromino::O:
            shape = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
            break;
        case Tetromino::T:
            shape = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
            break;
        case Tetromino::S:
            shape = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
            break;
        case Tetromino::Z:
            shape = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
            break;
        case Tetromino::J:
            shape = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
            break;
        case Tetromino::L:
            shape = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
            break;
    }

    // 应用旋转
    for (int i = 0; i < static_cast<int>(rotation); ++i) {
        for (auto& point : shape) {
            int x = point.x();
            int y = point.y();
            point.setX(-y);
            point.setY(x);
        }
    }

    return shape;
}

QColor TetrisGame::getTetrominoColor(Tetromino type) const
{
    switch (type) {
        case Tetromino::I: return QColor(0, 255, 255);    // 青色
        case Tetromino::O: return QColor(255, 255, 0);    // 黄色
        case Tetromino::T: return QColor(128, 0, 128);    // 紫色
        case Tetromino::S: return QColor(0, 255, 0);      // 绿色
        case Tetromino::Z: return QColor(255, 0, 0);      // 红色
        case Tetromino::J: return QColor(0, 0, 255);      // 蓝色
        case Tetromino::L: return QColor(255, 165, 0);    // 橙色
        default: return QColor(255, 255, 255);
    }
}

Tetromino TetrisGame::getRandomTetromino() const
{
    return static_cast<Tetromino>(QRandomGenerator::global()->bounded(0, 7));
}

bool TetrisGame::isValidPosition(const QVector<QPoint>& piece, const QPoint& pos) const
{
    return !checkCollision(piece, pos);
}

bool TetrisGame::checkCollision(const QVector<QPoint>& piece, const QPoint& pos) const
{
    for (const auto& point : piece) {
        int x = pos.x() + point.x();
        int y = pos.y() + point.y();

        // 检查边界
        if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
            return true;
        }

        // 检查是否与已放置的方块碰撞
        if (y >= 0 && m_board[y][x] != 0) {
            return true;
        }
    }
    return false;
}

void TetrisGame::spawnPiece()
{
    m_currentTetromino = m_nextTetromino;
    m_currentColor = m_nextColor;
    m_currentRotation = Rotation::North;
    m_currentPos = QPoint(BOARD_WIDTH / 2 - 1, 0);

    m_nextTetromino = getRandomTetromino();
    m_nextColor = getTetrominoColor(m_nextTetromino);

    emit pieceChanged();

    // 检查游戏结束
    if (checkCollision(getCurrentPiece(), m_currentPos)) {
        m_gameOver = true;
        m_gameTimer->stop();
        emit gameOverSignal();
    }
}

void TetrisGame::lockPiece()
{
    // 将当前方块锁定到游戏板
    for (const auto& point : getCurrentPiece()) {
        int x = m_currentPos.x() + point.x();
        int y = m_currentPos.y() + point.y();

        if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
            m_board[y][x] = 1;
        }
    }

    // 清除完整的行
    clearLines();

    // 生成新方块
    spawnPiece();

    emit boardChanged();
}

void TetrisGame::clearLines()
{
    int linesCleared = 0;

    for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
        bool fullLine = true;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (m_board[y][x] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            // 移除该行
            m_board.removeAt(y);
            // 在顶部添加空行
            m_board.prepend(QVector<int>(BOARD_WIDTH, 0));
            // 重新检查当前行
            ++y;
            ++linesCleared;
        }
    }

    if (linesCleared > 0) {
        // 更新分数
        int points[] = {0, 100, 300, 500, 800};
        m_score += points[linesCleared] * m_level;
        m_lines += linesCleared;

        // 更新等级
        updateLevel();

        emit scoreChanged(m_score);
        emit linesChanged(m_lines);
    }
}

void TetrisGame::updateLevel()
{
    int newLevel = (m_lines / 10) + 1;
    if (newLevel > m_level) {
        m_level = newLevel;
        // 加快下落速度
        m_dropInterval = qMax(100, 1000 - (m_level - 1) * 100);
        m_gameTimer->setInterval(m_dropInterval);
        emit levelChanged(m_level);
    }
}

void TetrisGame::updateBoard()
{
    emit boardChanged();
}