#include "sfScene.h"
#include "../Core/sfLogger.h"

std::vector<sf::Scene*>* sf::Scene::sAllScenePointers = nullptr;
std::unordered_map<std::string, sf::Scene*> sf::Scene::sAllScenes = {};
sf::Scene* sf::Scene::sCurrentScene = nullptr;

sf::Scene::Scene(const std::string& sceneName, bool startingScene) {
	mStartingScene = startingScene;
	mSceneName = sceneName;
	mSceneKeptAlive = false;
	if (sAllScenePointers == nullptr) {
		sAllScenePointers = new std::vector<sf::Scene*>();
	}

	sAllScenePointers->push_back(this);
}

void sf::Scene::_makeSceneUsable() {
	sAllScenes[mSceneName] = this;
}

const std::string& sf::Scene::getSceneName() {
	return mSceneName;
}

bool sf::Scene::isKeptAlive() {
	return mSceneKeptAlive;
}

bool sf::Scene::isStarting() {
	return mStartingScene;
}

void sf::Scene::keepAlive(bool keepSceenAlive) {
	mSceneKeptAlive = keepSceenAlive;
}

bool sf::Scene::changeScene(const std::string& sceneName, bool keepLastAlive) {
	Scene* lastScene = sCurrentScene;

	sCurrentScene = sAllScenes[sceneName];

	if (!hasSceneSelected()) {
		SF_CLOG("ERR: Scene \"" << sceneName << "\" not found!");

		sCurrentScene = lastScene;

		return false;
	}

	if (lastScene) {
		bool lastKeptAlive = true;

		// We don`t want to have zombie scenes taking up memeory on ram and vram, unless user has choose to
		if ((!lastScene->isKeptAlive() || keepLastAlive)) {
			lastScene->end();

			lastKeptAlive = false;
		}

		lastScene->keepAlive(lastKeptAlive);
	}

	if(!sCurrentScene->isKeptAlive())
		sCurrentScene->start();

	return true;
}

void sf::Scene::updateCurrentScene() {
	if (hasSceneSelected()) {
		sCurrentScene->update();
	}
}

void sf::Scene::lateUpdateCurrentScene() {
	if (hasSceneSelected())
		sCurrentScene->lateUpdate();
}

bool sf::Scene::hasSceneSelected() {
	return sCurrentScene != nullptr;
}

bool sf::Scene::hasAvailableScenes() {
	return !sAllScenes.empty();
}

void sf::Scene::endCurrentScene() {
	if (hasSceneSelected()) {
		sCurrentScene->end();
	}

	sCurrentScene = nullptr;
}

void sf::Scene::_makeAllScenesUsable() {
	SF_CDEBUG("All added scenes count: " << sAllScenePointers->size());

	for (Scene* scene : *sAllScenePointers) {
		scene->_makeSceneUsable();

		SF_CDEBUG("Scene \"" << scene->getSceneName() << "\", starting: " << (scene->isStarting() ? "true" : "false") << " @ " << scene);

		if (scene->isStarting())
			sf::Scene::changeScene(scene->getSceneName());
	}

	sAllScenePointers->clear();

	delete sAllScenePointers;
}