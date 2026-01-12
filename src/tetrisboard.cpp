#include "tetrisboard.h"
#include "tetrisgame.h"
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QRect>

TetrisBoard::TetrisBoard(TetrisGame *game, QWidget *parent)
    : QWidget(parent)
    , m_game(game)
{
    setMinimumSize(300, 600);
    setFocusPolicy(Qt::StrongFocus);
}

TetrisBoard::~TetrisBoard()
{
}

void TetrisBoard::setGame(TetrisGame *game)
{
    m_game = game;
    update();
}

void TetrisBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    if (!m_game) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景
    painter.fillRect(rect(), QColor(20, 20, 30));

    // 绘制游戏板
    drawBoard(painter);

    // 绘制网格
    drawGrid(painter);

    // 绘制下一个方块
    drawNextPiece(painter);
}

void TetrisBoard::drawBoard(QPainter &painter)
{
    if (!m_game) return;

    const QVector<QVector<int>>& board = m_game->getBoard();
    QVector<QPoint> currentPiece = m_game->getCurrentPiece();
    QColor currentColor = m_game->getCurrentPieceColor();
    QPoint currentPos = m_game->getCurrentPos();
    QPoint shadowPos = m_game->getShadowPos();

    // 绘制已放置的方块
    for (int y = 0; y < board.size(); ++y) {
        for (int x = 0; x < board[y].size(); ++x) {
            if (board[y][x] != 0) {
                QRect cell(x * cellSize(), y * cellSize(), cellSize(), cellSize());
                painter.fillRect(cell, QColor(100, 100, 150));
                painter.setPen(QColor(150, 150, 200));
                painter.drawRect(cell);
            }
        }
    }

    // 绘制阴影（如果阴影位置与当前位置不同）
    if (!m_game->isGameOver() && !currentPiece.isEmpty() && shadowPos != currentPos) {
        drawShadow(painter, currentPiece, shadowPos, currentColor);
    }

    // 绘制当前方块
    if (!m_game->isGameOver() && !currentPiece.isEmpty()) {
        drawPiece(painter, currentPiece, currentPos, currentColor);
    }
}

void TetrisBoard::drawPiece(QPainter &painter, const QVector<QPoint> &piece,
                           const QPoint &pos, const QColor &color)
{
    for (const auto& point : piece) {
        int x = (pos.x() + point.x()) * cellSize();
        int y = (pos.y() + point.y()) * cellSize();
        
        QRect cell(x, y, cellSize(), cellSize());
        
        // 绘制方块主体
        painter.fillRect(cell, color);
        
        // 绘制高光效果
        painter.setPen(QColor(255, 255, 255, 100));
        painter.drawLine(cell.topLeft(), cell.topRight());
        painter.drawLine(cell.topLeft(), cell.bottomLeft());
        
        // 绘制阴影效果
        painter.setPen(QColor(0, 0, 0, 100));
        painter.drawLine(cell.bottomRight(), cell.bottomLeft());
        painter.drawLine(cell.bottomRight(), cell.topRight());
        
        // 绘制边框
        painter.setPen(QColor(0, 0, 0));
        painter.drawRect(cell);
    }
}

void TetrisBoard::drawShadow(QPainter &painter, const QVector<QPoint> &piece,
                            const QPoint &pos, const QColor &color)
{
    for (const auto& point : piece) {
        int x = (pos.x() + point.x()) * cellSize();
        int y = (pos.y() + point.y()) * cellSize();
        
        QRect cell(x, y, cellSize(), cellSize());
        
        // 绘制半透明阴影
        QColor shadowColor = color;
        shadowColor.setAlpha(80);
        painter.fillRect(cell, shadowColor);
        
        // 绘制虚线边框
        QPen pen(QColor(255, 255, 255, 150), 1);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(cell);
    }
}

void TetrisBoard::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(QColor(50, 50, 70), 1));

    // 绘制垂直线
    for (int x = 0; x <= boardWidth(); ++x) {
        painter.drawLine(x * cellSize(), 0, x * cellSize(), boardHeight() * cellSize());
    }

    // 绘制水平线
    for (int y = 0; y <= boardHeight(); ++y) {
        painter.drawLine(0, y * cellSize(), boardWidth() * cellSize(), y * cellSize());
    }
}

void TetrisBoard::drawNextPiece(QPainter &painter)
{
    if (!m_game) return;

    QVector<QPoint> nextPiece = m_game->getNextPiece();
    QColor nextColor = m_game->getNextPieceColor();

    if (nextPiece.isEmpty()) return;

    // 计算下一个方块的显示位置
    int startX = boardWidth() * cellSize() + 20;
    int startY = 50;

    // 绘制标签
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(startX, startY - 10, "下一个:");

    // 绘制下一个方块
    for (const auto& point : nextPiece) {
        int x = startX + point.x() * nextPieceSize();
        int y = startY + point.y() * nextPieceSize();
        
        QRect cell(x, y, nextPieceSize(), nextPieceSize());
        
        painter.fillRect(cell, nextColor);
        painter.setPen(QColor(0, 0, 0));
        painter.drawRect(cell);
    }
}

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (!m_game) {
        QWidget::keyPressEvent(event);
        return;
    }

    // 只有在游戏开始后才能操作方块
    if (!m_game->isGameStarted()) {
        QWidget::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
        // 标准方向键
        case Qt::Key_Left:
            m_game->moveLeft();
            break;
        case Qt::Key_Right:
            m_game->moveRight();
            break;
        case Qt::Key_Down:
            m_game->moveDown();
            break;
        case Qt::Key_Up:
            m_game->rotate();
            break;
        case Qt::Key_Space:
            m_game->hardDrop();
            break;
        // Vim风格按键
        case Qt::Key_H:
            m_game->moveLeft();
            break;
        case Qt::Key_L:
            m_game->moveRight();
            break;
        case Qt::Key_J:
            m_game->moveDown();
            break;
        case Qt::Key_K:
            m_game->rotate();
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
    }
}

int TetrisBoard::cellSize() const
{
    return qMin(width() / (boardWidth() + 4), height() / boardHeight());
}

int TetrisBoard::boardWidth() const
{
    return 10;
}

int TetrisBoard::boardHeight() const
{
    return 20;
}

int TetrisBoard::nextPieceSize() const
{
    return cellSize();
}