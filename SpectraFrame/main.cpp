#include "Core/sfWindow.h"
#include "Scene/sfScene.h"

class ApplicationWindow : public sf::Window {
public:
  virtual void update() {
    sf::Scene::updateCurrentScene();
  }

  virtual void lateUpdate() {
    sf::Scene::lateUpdateCurrentScene();
  }

  virtual void end() {
    sf::Scene::endCurrentScene();
  }
};

int main() {
  ApplicationWindow appWindow;

  appWindow
    .useVulkanDebugger(true)
    .create("SpectraFrame Engine App", 800, 600);

  sf::Scene::_makeAllScenesUsable();

  appWindow
    .runLoop()
    .destroy();

  return 0;
}
