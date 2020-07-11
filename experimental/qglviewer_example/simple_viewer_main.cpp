#include <QGLViewer/qglviewer.h>
#include <QtWidgets/qapplication.h>
#include <glog/logging.h>

namespace dungeon {

void DrawSpiral() {
  constexpr int kNumSteps = 1000;
  constexpr int kNumTurns = 10;
  constexpr float p1 = 1.0;
  constexpr float p2 = 0.9f;
  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i < kNumSteps; ++i) {
    const float ratio = i / static_cast<float>(kNumSteps);
    const float angle = 2.0 * M_PI * kNumTurns * ratio;
    const float altitude = ratio - 0.5f;
    const float nor = 0.5f;
    const float up = std::sqrt(1.0 - nor * nor);
    glColor3f(1.0 - 0.5 * ratio * ratio, 1.0 - ratio, ratio);
    glNormal3f(nor * std::cos(angle), up, nor * std::sin(angle));
    glVertex3f(p1 * std::cos(angle), altitude, p1 * std::sin(angle));
    glVertex3f(p2 * std::cos(angle), altitude + 0.01f, p2 * std::sin(angle));
  }
  glEnd();
}

// A simplest possible instantiation of the QGLViewer class.
class SimpleViewer : public QGLViewer {
 protected:
  virtual void draw() { DrawSpiral(); }
  virtual void init() {}
  virtual QString helpString() const {
    return QString("Normally this would print a help string.");
  }
};

}  // namespace dungeon

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  QApplication application(argc, argv);

  dungeon::SimpleViewer viewer;
  viewer.setWindowTitle("Viewer");
  viewer.show();
  return application.exec();
}
