#include "game.h"
#include <QDebug>
#include <QtMath>

Game::Game(QWidget *parent) :
    QOpenGLWidget(parent),
    shadersCompiled(false),
    camXRot(0), camYRot(-90.0f), camZRot(0)
{
    QSurfaceFormat format;
    format.setSamples(4);
    setFormat(format);
    this->setMinimumSize(100, 100);
    this->resize(parent->size());
    this->setFocus();
    timer.start();
}

Game::~Game()
{
    planetsProgram->release();
    delete planetsProgram;
    // temp
    delete tex;
}

void Game::setXRotation(float angle)
{
    if (angle != camXRot) {
        camXRot = angle;
    }
}

void Game::setYRotation(float angle)
{
    if (angle != camYRot) {
        camYRot = angle;
    }
}

void Game::setZRotation(float angle)
{
    if (angle != camZRot) {
        camZRot = angle;
    }
}

void Game::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_MULTISAMPLE);

    Vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&Vao);
    planetsProgram = new QOpenGLShaderProgram;

    projectionMat.setToIdentity();
    projectionMat.perspective(45.0f, float(this->width()) / float(this->height()), 0.01f, 100.0f);
//    projectionMat.frustum(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); // orthographic projection mode

    planetVbo.create();

    camPos   = QVector3D(0.0f, 0.0f,  3.0f);
    camFront = QVector3D(0.0f, 0.0f, -1.0f);
    camUp    = QVector3D(0.0f, 1.0f,  0.0f);
    camSpeed = 0.05f;
    rotationSpeed = 0.1f;
    oldTime = timer.elapsed();
}

void Game::resizeGL(int w, int h)
{
    projectionMat.setToIdentity();
    projectionMat.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void Game::drawSphere(float radius, float x, float y, float z)
{
    // compile shaders
    if (!shadersCompiled)
    {
        bool noerror = true;
        noerror = planetsProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sphereshaderV.vert");
        if (!noerror) {qDebug() << planetsProgram->log();}
        noerror = planetsProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sphereshader.frag");
        if (!noerror) {qDebug() << planetsProgram->log();}
        planetsProgram->bindAttributeLocation("position", 0);
        planetsProgram->bindAttributeLocation("texCoord", 2);
        noerror = planetsProgram->link();
        if (!noerror) {qDebug() << planetsProgram->log();}
        noerror = planetsProgram->bind();
        if (!noerror) {qDebug() << planetsProgram->log();}
        planetsProgram->setUniformValue("ourTexture1", 0);
        shadersCompiled = true;
        // temp
        tex = new QOpenGLTexture(QImage(QString(":/planets/oceaniczna.png")));
    }
    QMatrix4x4 modelMat;
    modelMat.setToIdentity();
    modelMat.translate(x, y, z);
    GLsphere ocean;
    ocean.create(radius);
    ocean.setTexture(tex);

    planetVbo.bind();
    planetVbo.allocate(ocean.constData(),ocean.count()*sizeof(GLfloat));
    for (int i = 0; i < ocean.count(); i++)
    {
        if (ocean.constData()[i] > 2*radius or ocean.constData()[i] < -2*radius)
            qDebug() << i << ocean.constData()[i];
    }

    planetsProgram->setUniformValue("model",modelMat);
    planetsProgram->setUniformValue("view",viewMat);
    planetsProgram->setUniformValue("projection",projectionMat);

    this->glEnableVertexAttribArray(0);
    this->glEnableVertexAttribArray(2);
    this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    this->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3*sizeof(GLfloat)));


    ocean.getTexture()->bind();
    glDrawArrays(GL_TRIANGLES, 0, ocean.count());
    planetVbo.release();
//    delete ocean.getTexture();
}

void Game::drawGeosphere(float x, float y, float z)
{
    // compile shaders
    if (!shadersCompiled)
    {
        bool noerror = true;
        noerror = planetsProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sphereshaderV.vert");
        if (!noerror) {qDebug() << planetsProgram->log();}
        noerror = planetsProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sphereshader.frag");
        if (!noerror) {qDebug() << planetsProgram->log();}
        planetsProgram->bindAttributeLocation("position", 0);
        planetsProgram->bindAttributeLocation("texCoord", 2);
        noerror = planetsProgram->link();
        if (!noerror) {qDebug() << planetsProgram->log();}
        noerror = planetsProgram->bind();
        if (!noerror) {qDebug() << planetsProgram->log();}
        planetsProgram->setUniformValue("ourTexture1", 0);
        shadersCompiled = true;
        // temp
        tex = new QOpenGLTexture(QImage(QString(":/planets/oceaniczna.png")));
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    QMatrix4x4 modelMat;
    modelMat.setToIdentity();
    modelMat.translate(x, y, z);
    QVector<GLfloat> gsphere;
    GeometryProvider g;
    g.geosphere(gsphere, GeometryProvider::Four, 5, 0);
    g.texturize(GeometryProvider::Geosphere, gsphere, 5, 3, 0);

    planetVbo.bind();
    planetVbo.allocate(gsphere.data(),gsphere.size()*sizeof(GLfloat));

    planetsProgram->setUniformValue("model",modelMat);
    planetsProgram->setUniformValue("view",viewMat);
    planetsProgram->setUniformValue("projection",projectionMat);

    this->glEnableVertexAttribArray(0);
    this->glEnableVertexAttribArray(2);
    this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    this->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3*sizeof(GLfloat)));

    tex->bind();
    glDrawArrays(GL_TRIANGLES, 0, gsphere.count());
    planetVbo.release();
//    delete ocean.getTexture();
}

void Game::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    viewMat.setToIdentity();
    viewMat.lookAt(camPos, camPos + camFront, camUp);

    QOpenGLVertexArrayObject::Binder vaoBinder(&Vao);

//    drawSphere(1.0f, 0.0f, 0.0f, 0.0f);
    drawGeosphere(0.0f, 0.0f, 0.0f);
    //drawSphere(0.5f, sinf(qDegreesToRadians(float((timer.elapsed()-oldTime)/50))), cosf(qDegreesToRadians(float((timer.elapsed()-oldTime)/50))), 0.0f);
    //oldTime=timer.elapsed();
    drawSphere(2.0f, 5.0f, 3.0f, 0.0f);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    lastCursorPos = event->pos();
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastCursorPos.x();
    int dy = event->y() - lastCursorPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(camXRot + rotationSpeed * dy);
        setYRotation(camYRot + rotationSpeed * dx);
    }
//    else if (event->buttons() & Qt::RightButton) { // Hey I just met you
//        setXRotation(m_xRot + 8 * dy);                and this is crazy
//        setZRotation(m_zRot + 8 * dx);                here is my event
//    }                                                 so fix me maybe
    lastCursorPos = event->pos();
    QVector3D front;
    front.setX(cosf(qDegreesToRadians(camYRot)) * cosf(qDegreesToRadians(camXRot)));
    front.setY(sinf(qDegreesToRadians(camXRot)));
    front.setZ(sinf(qDegreesToRadians(camYRot)) * cosf(qDegreesToRadians(camXRot)));
    camFront = front.normalized();
    this->update();
}

void Game::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta();
    if (!numPixels.isNull())
    {
        camPos += camSpeed*float(numPixels.y()) * camFront;
    }
    else
    {
        camPos += camSpeed*float(numDegrees.y())/100.0f * camFront;
    }
    event->accept();
    this->update();
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        emit exitToMenu();
    }
    else if (event->key() == Qt::Key_W)
    {
        camPos += camSpeed * camFront;
    }
    else if (event->key() == Qt::Key_S)
    {
        camPos -= camSpeed * camFront;
    }
    if (event->key() == Qt::Key_D)
    {
        camPos += QVector3D::normal(camFront, camUp) * camSpeed;
    }
    else if (event->key() == Qt::Key_A)
    {
        camPos -= QVector3D::normal(camFront, camUp) * camSpeed;
    }
    if (event->key() == Qt::Key_R)
    {
        camPos   = QVector3D(0.0f, 0.0f,  3.0f);
        camFront = QVector3D(0.0f, 0.0f, -1.0f);
        camUp    = QVector3D(0.0f, 1.0f,  0.0f);
        camXRot=0;
        camYRot=0;
        camZRot=0;
    }
    else
        event->ignore();
    event->accept();
    this->update();
}
