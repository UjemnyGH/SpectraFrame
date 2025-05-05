#pragma once
#ifndef _SPECTRAFRAME_SCENE_
#define _SPECTRAFRAME_SCENE_

#include "../Core/sfVkCommon.h"

namespace sf {
	class Scene {
	private:
		std::string mSceneName;

		bool mSceneKeptAlive;
		bool mStartingScene;

	public:
		static std::vector<sf::Scene*>* sAllScenePointers;

		static std::unordered_map<std::string, Scene*> sAllScenes;

		static Scene* sCurrentScene;

		/**
		 * @brief 
		 * @param sceneName 
		 * @param startingScene NOTE: Only one scene can be staring scene, default false
		 */
		Scene(const std::string& sceneName, bool startingScene = false);

		/**
		 * @brief DO NOT CALL THIS FUNCTION
		 */
		void _makeSceneUsable();

		virtual void start() {}

		virtual void update() {}

		virtual void lateUpdate() {}

		virtual void end() {}
		
		const std::string& getSceneName();

		bool isKeptAlive();

		bool isStarting();

		void keepAlive(bool keepSceenAlive);

		// Statics
		static bool changeScene(const std::string& sceneName, bool keepLastAlive = false);

		static void updateCurrentScene();

		static void lateUpdateCurrentScene();

		static bool hasSceneSelected();

		static bool hasAvailableScenes();

		static void endCurrentScene();

		static void _makeAllScenesUsable();
	};
}

#endif