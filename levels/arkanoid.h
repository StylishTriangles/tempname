#ifndef ARKANOID_H
#define ARKANOID_H

#include <QColor>
#include <QElapsedTimer>
#include <QHash>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPair>
#include <QPoint>
#include <QSize>
#include <QThread>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include "include/neural.h"

namespace Aurora {

class ArkanoidWorker;
class ArkanoidBrick;
class ArkanoidWidget;

class ArkanoidWorker : public QObject
{
    Q_OBJECT
public:
    ArkanoidWorker(ArkanoidWidget* parent, int threadID) :
        p(parent), myThreadID(threadID), initialized(false) {}
    friend class ArkanoidWidget;
public slots:
    void onTick();
    void restart();
private:
    void onTick(int threadID);
    void reset();

    ArkanoidWidget* p;
    int myThreadID;
    QVector<ArkanoidBrick> levelData;
    QVector<QVector<double>> neuralInputs;
    QLabel qlGen, qlIndex, qlScore;
    QPoint ballPos;
    QPoint vausPos;
    QSize vausSize;
    QSize defBrickSize;
    int ballRadius;
    int lives;
    int vx;
    int ballVy, ballVx;
    int maxVx, maxVy;
    int score;
    int scoreMult;
    int totalBricks;
    int msTimeLimit;
    int constTickTime; // only used in neural mode used to simulate 10 ms frames
    int tickCount;
    int tickCountLimit;
    bool keyLT, keyRT;
    bool gameOver;
    bool userTerminate;
    bool debugNeuralInputs;
    bool normalSpeed;
    bool initialized;
    int iter;
};

struct ArkanoidBrick
{
    ArkanoidBrick() = default;
    ArkanoidBrick(QColor bc, QColor fc, QSize sz, QPoint position, int toughness) :
        borderCol(bc), faceCol(fc),
        size(sz), pos(position),
        tgh(toughness), destroyed(false) {}
    QColor borderCol;
    QColor faceCol;
    QSize size;
    QPoint pos;
    short tgh;
    bool destroyed;

    QRect asRect() {
        return QRect(pos, size);
    }
};

class ArkanoidWidget final : public QWidget
{
    Q_OBJECT
public:
    ArkanoidWidget(QWidget* parent = 0);
    ~ArkanoidWidget();
    const int DEF_WIDTH = 960;
    const int DEF_HEIGHT = 720;
    const int BUCKET_WIDTH = (DEF_WIDTH+4)/8;
    const int BUCKET_HEIGHT = (DEF_HEIGHT+4)/8;
    const int NEURAL_NET_INPUT_RADIUS = 40;
    void start();
    void runAsNeuralNetwork();
    void setThreadCount(int count) {threadCount = count;}
    QVector<QVector<int>> workerData;
    friend class ArkanoidWorker;

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *qke) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *qke) Q_DECL_OVERRIDE;
private:
    void setNeuralInputs();
    void levelGen(int id = 0);

private slots:
    void onTick();
    void neuroTick();
    void reset();

private:
    struct NeuralState {
        NeuralState() :
            generation(0), population(defPopulation), index(0) {}
        int generation;
        int population;
        int index;
        static const int defPopulation = 400;
    };
    std::mt19937 rng;
    NeuralState nst;
    QVector<ArkanoidBrick*> levelData;
    QVector<QVector<double>> neuralInputs;
    QVector<Aurora::NeuralNetwork> vnn;
    QElapsedTimer elt;
    QTimer tle, vft; // VeryFastTimer
    qint64 tx;
    QLabel qlGen, qlIndex, qlScore;
    QPoint ballPos;
    QPoint vausPos;
    QSize vausSize;
    QSize defBrickSize;
    int threadCount;
    int ballRadius;
    int lives;
    int vx;
    int ballVy, ballVx;
    int maxVx, maxVy;
    int score;
    int scoreMult;
    int totalBricks;
    int msTimeLimit;
    int constTickTime; // only used in neural mode used to simulate 10 ms frames
    int tickCount;
    int tickCountLimit;
    bool neuralMode;
    bool keyLT, keyRT;
    bool gameOver;
    bool userTerminate;
    bool debugNeuralInputs;
    bool normalSpeed;
    bool initialized;
    bool resetting;
};

} // Aurora
#endif // ARKANOID_H
