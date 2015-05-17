#pragma once
#include "GameManager.hpp"
#include "GameState.hpp"

namespace Pocket {
	class GameStateManager;
	class Window;
    class Timer;
    class Engine {
	public:
		Engine();
		~Engine();

		template<class T>
		void Start(
#ifdef IPHONE
                bool landscape = false
#else
                int width = 1280, int height=720, bool fullScreen=false
#endif
        );

	private:
		bool fullScreen;

		void StartLoop(IGameState* rootState, int width, int height, bool fullScreen);
		bool Update(float dt);
		void Render();

		Window* window;

        
        void Loop(bool* exit);
        
        bool firstFrame;
        Timer* timer;
        
        GameManager manager;
        IGameState* rootState;
        
        friend class Thread;
	};
}


template<class T>
void Pocket::Engine::Start(
#ifdef IPHONE
                         bool landscape
#else
                         int width, int height, bool fullScreen
#endif
) {
#ifdef IPHONE
	StartLoop(new T(),0,0,landscape);
#else
    StartLoop(new T(),width,height,fullScreen);
#endif
}