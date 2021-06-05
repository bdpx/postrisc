#pragma once

#include <mutex>
#include <QWindow>
#include <QMouseEvent>
#include <QGraphicsView>

namespace postrisc {

class TargetSystem;

/***************************************************************************
* key presses ring buffer which gui writes, emulator reads
***************************************************************************/
class KeyRing {
public:
    void addKeyToQueue(bool pressed, Qt::Key keyCode);
    bool GetKey(bool& pressed, Qt::Key& keyCode);

public:
    // singleton
    static KeyRing s_KeyRing;
private:
    static const size_t KEYQUEUE_SIZE = 32;
    std::mutex   m_lock;
    struct {
        bool     pressed;
        Qt::Key  keyCode;
    } m_KeyQueue[KEYQUEUE_SIZE];
    u32     m_KeyQueueWriteIndex = 0;
    u32     m_KeyQueueReadIndex = 0;
};

/***************************************************************************
* emulator frame buffer
***************************************************************************/
class FrameBufferWindow : public QGraphicsView {
Q_OBJECT

public:
    using Super = QGraphicsView;

    explicit FrameBufferWindow(TargetSystem& targetSystem, QWidget *parent = 0);
    ~FrameBufferWindow();

    void do_resize() { _do_resize = true; }

signals:
    void finished(QImage &);

public slots:
    void updateGbScreen(void);
    void applyImage(const QImage& img);

    //void renderLater(void);
    //void renderNow(void);

protected:
    //bool event(QEvent *event) override;
    //void exposeEvent(QExposeEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QWidget *_parent;
    std::unique_ptr<QGraphicsScene> _scene;
    bool _do_resize = false;
    u32  m_frame;
    bool m_is_mouse_left_button_pressed;
    TargetSystem& m_TargetSystem;
};

} // namespace postrisc
