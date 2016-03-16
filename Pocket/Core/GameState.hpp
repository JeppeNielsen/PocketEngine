//
//  GameState.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 9/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include "Property.hpp"
#include "InputManager.hpp"
#include "GameManager.hpp"

namespace Pocket {

    class IGameState : public IInputManagerIterator {
    public:
        IGameState();
        virtual ~IGameState();
    
        virtual void DoInitialize(GameManager* manager) = 0;
        virtual void DoUpdate(float dt) = 0;
        virtual void DoRender() = 0;
        virtual void UpdateInput(IInputManagerUpdater* inputManagerUpdater) = 0;
        
        InputManager Input;
        
    };
    
    template<class T>
    class GameState : public IGameState {
    public:
        GameState();
        virtual ~GameState();
        
        void DoInitialize(GameManager* manager);
        void DoUpdate(float dt);
        void DoRender();
        void UpdateInput(IInputManagerUpdater* inputManagerUpdater);
        
        
    protected:
        
        virtual void Initialize() = 0;
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
        
        virtual void Enabled();
        virtual void Disabled();
        
        template<class S>
        void AddState(std::string name);
        
    public:
        Property<std::string> CurrentState;
    protected:
        T& Parent();
        
        GameManager& Manager();
        
    private:
        T* parent;
        
        typedef std::map<std::string, GameState*> States;
        States states;
        
        void CheckState();
        
        GameState* currentState;
        GameState* wantedState;
        
        GameManager* manager;
    };
}

template<class T>
Pocket::GameState<T>::GameState() : currentState(0), parent(0) {
    CurrentState = "";
    CurrentState.Changed.Bind([this] () {
        typename States::iterator current = states.find(CurrentState);
        if (current!=states.end()) {
            wantedState = current->second;
        } else {
            wantedState = 0;
        }
    });
}

template<class T>
Pocket::GameState<T>::~GameState() {
    for (typename States::iterator it = states.begin(); it!=states.end(); ++it) {
        delete it->second;
    }
    states.clear();
}


template<class T>
void Pocket::GameState<T>::DoInitialize(GameManager* manager) {
    this->manager = manager;
    Initialize();
    for (typename States::iterator it = states.begin(); it!=states.end(); ++it) {
        it->second->DoInitialize(manager);
    }
}

template<class T>
void Pocket::GameState<T>::DoUpdate(float dt) {
    CheckState();
    Update(dt);
    if (currentState) {
        currentState->DoUpdate(dt);
    }
}

template<class T>
void Pocket::GameState<T>::DoRender() {
    Render();
    if (currentState) {
        currentState->DoRender();
    }
}


template<class T>
void Pocket::GameState<T>::Enabled() {}

template<class T>
void Pocket::GameState<T>::Disabled() {}

template<class T>
template<class S>
void Pocket::GameState<T>::AddState(std::string name) {
    typename States::iterator it = states.find(name);
    if (it!=states.end()) {
        delete it->second;
        states.erase(it);
    }
    S* state = new S();
    state->parent = (T*)this;
    states[name] = state;
}

template<class T>
void Pocket::GameState<T>::CheckState() {
    if (currentState==wantedState) return;
    if (currentState) {
        currentState->Disabled();
    }
    currentState = wantedState;
    if (currentState) {
        currentState->Enabled();
    }
}

template<class T>
T& Pocket::GameState<T>::Parent() { return *parent; }

template<class T>
Pocket::GameManager& Pocket::GameState<T>::Manager() { return *manager; }

template<class T>
void Pocket::GameState<T>::UpdateInput(IInputManagerUpdater* inputManagerUpdater) {
    inputManagerUpdater->UpdateInputManager(&Input);
    if (currentState) {
        currentState->UpdateInput(inputManagerUpdater);
    }
}



