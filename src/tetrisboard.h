#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

class TetrisGame;

class TetrisBoard : public QWidget
{
    Q_OBJECT

public:
    explicit TetrisBoard(TetrisGame *game, QWidget *parent = nullptr);
    ~TetrisBoard();

    void setGame(TetrisGame *game);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    TetrisGame *m_game;

    // 绘制相关
    void drawBoard(QPainter &painter);
    void drawPiece(QPainter &painter, const QVector<QPoint> &piece, 
                   const QPoint &pos, const QColor &color);
    void drawGrid(QPainter &painter);
    void drawNextPiece(QPainter &painter);

    // 尺寸计算
    int cellSize() const;
    int boardWidth() const;
    int boardHeight() const;
    int nextPieceSize() const;
};

#endif // TETRISBOARD_H