#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "LayoutSystem.hpp"

using namespace Pocket;

/*
struct Layouter {
    enum class LayoutMode {
        Horizontal,
        Vertical,
    };

    Property<LayoutMode> ChildrenLayoutMode;
    
    Property<Vector2> Min;
    Property<Vector2> Max;
    Property<Vector2> Desired;
    
    DirtyProperty<Vector2> GlobalMin;
    DirtyProperty<Vector2> GlobalMax;
    DirtyProperty<Vector2> GlobalDesired;
    
    void SetAll(Vector2 size) {
        Min = size;
        Max = size;
        Desired = size;
    }
};


struct LayouterSystem : public GameSystem<Layouter, Transform, Sizeable> {

    std::vector<GameObject*> dirtyObjects;

    void ObjectAdded(GameObject* object) {
        Layouter* layouter = object->GetComponent<Layouter>();
        
        layouter->GlobalMin.Method = [layouter, object] (Vector2& value) {
            value = DoLayout(layouter, object,
            [] (Layouter* l) {
                return l->Min();
            }, [] (Layouter* l) {
                return l->GlobalMin();
            });
        };
        
        layouter->GlobalMax.Method = [layouter, object] (Vector2& value) {
            value = DoLayout(layouter, object, [] (Layouter* l) { return l->Max(); }, [] (Layouter* l) { return l->GlobalMax(); });
        };
        
        layouter->GlobalDesired.Method = [layouter, object] (Vector2& value) {
            value = DoLayout(layouter, object, [] (Layouter* l) { return l->Desired(); }, [] (Layouter* l) { return l->GlobalDesired(); });
        };
    
        layouter->Min.Changed.Bind(this, &LayouterSystem::MinChanged, layouter);
        layouter->Max.Changed.Bind(this, &LayouterSystem::MaxChanged, layouter);
        layouter->Desired.Changed.Bind(this, &LayouterSystem::DesiredChanged, layouter);
        
        layouter->GlobalMin.MakeDirty();
        layouter->GlobalMax.MakeDirty();
        layouter->GlobalDesired.MakeDirty();
        
        layouter->GlobalMin.HasBecomeDirty.Bind(this, &LayouterSystem::GlobalMinDirty, object);
        layouter->GlobalMax.HasBecomeDirty.Bind(this, &LayouterSystem::GlobalMaxDirty, object);
        layouter->GlobalDesired.HasBecomeDirty.Bind(this, &LayouterSystem::GlobalDesiredDirty, object);
        
        object->Parent.Changed.Bind(this, &LayouterSystem::ParentChanged, object);
        
        object->GetComponent<Sizeable>()->Size.Changed.Bind(this, &LayouterSystem::SizeChanged, object);
        
        dirtyObjects.push_back(object);
        
        GameObject* current = object->Parent;
        Layouter* currentLayouter = current ? current->GetComponent<Layouter>() : nullptr;
        if (currentLayouter) {
            dirtyObjects.push_back(current);
        }
    }
    
    void ObjectRemoved(GameObject* object) {
        Layouter* layouter = object->GetComponent<Layouter>();
        layouter->GlobalMin.Method = nullptr;
        layouter->GlobalMax.Method = nullptr;
        layouter->GlobalDesired.Method = nullptr;
        
        layouter->Min.Changed.Unbind(this, &LayouterSystem::MinChanged, layouter);
        layouter->Max.Changed.Unbind(this, &LayouterSystem::MaxChanged, layouter);
        layouter->Desired.Changed.Unbind(this, &LayouterSystem::DesiredChanged, layouter);
        
        layouter->GlobalMin.HasBecomeDirty.Unbind(this, &LayouterSystem::GlobalMinDirty, object);
        layouter->GlobalMax.HasBecomeDirty.Unbind(this, &LayouterSystem::GlobalMaxDirty, object);
        layouter->GlobalDesired.HasBecomeDirty.Unbind(this, &LayouterSystem::GlobalDesiredDirty, object);
        
        object->Parent.Changed.Unbind(this, &LayouterSystem::ParentChanged, object);
        
        object->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &LayouterSystem::SizeChanged, object);
        
        GameObject* current = object->Parent;
        Layouter* currentLayouter = current ? current->GetComponent<Layouter>() : nullptr;
        if (currentLayouter) {
            dirtyObjects.push_back(current);
        }
    }
    
    
    void MinChanged(Layouter* layouter) {
        layouter->GlobalMin.MakeDirty();
    }
    
    void MaxChanged(Layouter* layouter) {
        layouter->GlobalMax.MakeDirty();
    }
    
    void DesiredChanged(Layouter* layouter) {
        layouter->GlobalDesired.MakeDirty();
    }
    
    void GlobalMinDirty(GameObject* object) {
        GameObject* parent = object->Parent;
        if (!object) return;
        Layouter* parentLayouter = parent->GetComponent<Layouter>();
        if (parentLayouter) {
            parentLayouter->GlobalMin.MakeDirty();
        }
        dirtyObjects.push_back(object);
    }
    
    void GlobalMaxDirty(GameObject* object) {
        GameObject* parent = object->Parent;
        if (!object) return;
        Layouter* parentLayouter = parent->GetComponent<Layouter>();
        if (parentLayouter) {
            parentLayouter->GlobalMax.MakeDirty();
        }
        dirtyObjects.push_back(object);
    }
    
    void GlobalDesiredDirty(GameObject* object) {
        GameObject* parent = object->Parent;
        if (!object) return;
        Layouter* parentLayouter = parent->GetComponent<Layouter>();
        if (parentLayouter) {
            parentLayouter->GlobalDesired.MakeDirty();
        }
        dirtyObjects.push_back(object);
    }
    
    void ParentChanged(GameObject* object) {
        GameObject* old = object->Parent.PreviousValue();
        GameObject* current = object->Parent;
        
        Layouter* oldLayouter = old ? old->GetComponent<Layouter>() : nullptr;
        Layouter* currentLayouter = current ? current->GetComponent<Layouter>() : nullptr;
        
        if (oldLayouter) {
            dirtyObjects.push_back(old);
        }
        if (currentLayouter) {
            dirtyObjects.push_back(current);
        }
    }
    
    void SizeChanged(GameObject* object) {
        dirtyObjects.push_back(object);
        Layouter* layouter = object->GetComponent<Layouter>();
        //layouter->GlobalMin.MakeDirty();
        //layouter->GlobalMax.MakeDirty();
        //layouter->GlobalDesired.MakeDirty();
    }
    
    static Vector2 DoLayout(Layouter* layouter, GameObject* object,
        const std::function<Vector2(Layouter* layouter)>& localGetter,
        const std::function<Vector2(Layouter* layouter)>& globalGetter
    ) {
        int count = 0;
        for(auto child : object->Children()) {
            Layouter* childLayouter = child->GetComponent<Layouter>();
            if (!childLayouter) continue;
            count++;
        }
        if (count == 0) {
            return localGetter(layouter);
        }
    
        //Vector2 layoutSize = localGetter(layouter);//object->GetComponent<Sizeable>()->Size;
        if (layouter->ChildrenLayoutMode == Layouter::LayoutMode::Horizontal) {
            Vector2 size { 0, localGetter(layouter).y };
            for(auto child : object->Children()) {
                Layouter* childLayouter = child->GetComponent<Layouter>();
                if (!childLayouter) continue;
                Vector2 childSize = globalGetter(childLayouter);
                size.x += childSize.x;
                size.y = std::max(size.y, childSize.y);
            }
            //size.x = std::max(layoutSize.x, size.x);
            return size;
        } else {
            Vector2 size { localGetter(layouter).x, 0 };
            for(auto child : object->Children()) {
                Layouter* childLayouter = child->GetComponent<Layouter>();
                if (!childLayouter) continue;
                Vector2 childSize = globalGetter(childLayouter);
                size.x = std::max(size.x, childSize.x);
                size.y += childSize.y;
            }
            //size.y = std::max(layoutSize.y, size.y);
            return size;
        }
    }
    
    void Update(float dt) {
        while (!dirtyObjects.empty()) {
            std::vector<GameObject*> temp = dirtyObjects;
            dirtyObjects.clear();
            for(auto o : temp) {
                CalcLayout(o);
            }
        }
    }
    
    void CalcLayout(GameObject* object) {
        Vector2 size = object->GetComponent<Sizeable>()->Size;
        Layouter* layouter = object->GetComponent<Layouter>();
        
        if (layouter->ChildrenLayoutMode == Layouter::LayoutMode::Horizontal) {
            if (layouter->GlobalMin().x>=size.x) {
                float x = 0;
                for(auto child : object->Children()) {
                    Layouter* childLayouter = child->GetComponent<Layouter>();
                    Sizeable* childSizable = child->GetComponent<Sizeable>();
                    Transform* childTransform = child->GetComponent<Transform>();
                    if (!childLayouter || !childSizable || !childTransform) continue;
                    
                    Vector2 childSize = { childLayouter->GlobalMin().x, size.y };
                    
                    childSizable->Size = childSize;
                    childTransform->Position = {x,0};
                    x+=childSize.x;
                }
            } else if (layouter->GlobalDesired().x>=size.x) {
                float desiredMargin = layouter->GlobalDesired().x - layouter->GlobalMin().x;
                float fraction = (size.x - layouter->GlobalMin().x) / desiredMargin;
                float x = 0;
                for(auto child : object->Children()) {
                    Layouter* childLayouter = child->GetComponent<Layouter>();
                    Sizeable* childSizable = child->GetComponent<Sizeable>();
                    Transform* childTransform = child->GetComponent<Transform>();
                    if (!childLayouter || !childSizable || !childTransform) continue;
                    
                    float minWidth = childLayouter->GlobalMin().x;
                    float desWidth = childLayouter->GlobalDesired().x;
                    
                    Vector2 childSize = { minWidth + (desWidth - minWidth) * fraction, size.y };
                    
                    childSizable->Size = childSize;
                    childTransform->Position = {x,0};
                    x+=childSize.x;
                }
            } else {
                float maxMargin = layouter->GlobalMax().x - layouter->GlobalDesired().x;
                float fraction = maxMargin > -0.00001f && maxMargin < 0.00001f ? 1 : (size.x - layouter->GlobalDesired().x) / maxMargin;
                if (fraction>1) fraction = 1;
                
                float x = 0;
                for(auto child : object->Children()) {
                    Layouter* childLayouter = child->GetComponent<Layouter>();
                    Sizeable* childSizable = child->GetComponent<Sizeable>();
                    Transform* childTransform = child->GetComponent<Transform>();
                    if (!childLayouter || !childSizable || !childTransform) continue;
                    
                    float desWidth = childLayouter->GlobalDesired().x;
                    float maxWidth = childLayouter->GlobalMax().x;
                    
                    Vector2 childSize = { desWidth + (maxWidth - desWidth) * fraction, size.y };
                    
                    childSizable->Size = childSize;
                    childTransform->Position = {x, 0};
                    x+=childSize.x;
                }
            }
        } else {
        
            if (layouter->GlobalMin().y>=size.y) {
                float y = 0;
                for(auto child : object->Children()) {
                    Layouter* childLayouter = child->GetComponent<Layouter>();
                    Sizeable* childSizable = child->GetComponent<Sizeable>();
                    Transform* childTransform = child->GetComponent<Transform>();
                    if (!childLayouter || !childSizable || !childTransform) continue;
                    
                    Vector2 childSize = { size.x, childLayouter->GlobalMin().y };
                    
                    childSizable->Size = childSize;
                    childTransform->Position = {0,y};
                    y+=childSize.y;
                }
            } else if (layouter->GlobalDesired().y>=size.y) {
                float desiredMargin = layouter->GlobalDesired().y - layouter->GlobalMin().y;
                float fraction = (size.y - layouter->GlobalMin().y) / desiredMargin;
                float y = 0;
                for(auto child : object->Children()) {
                    Layouter* childLayouter = child->GetComponent<Layouter>();
                    Sizeable* childSizable = child->GetComponent<Sizeable>();
                    Transform* childTransform = child->GetComponent<Transform>();
                    if (!childLayouter || !childSizable || !childTransform) continue;
                    
                    float minWidth = childLayouter->GlobalMin().y;
                    float desWidth = childLayouter->GlobalDesired().y;
                    
                    Vector2 childSize = { size.x, minWidth + (desWidth - minWidth) * fraction };
                    
                    childSizable->Size = childSize;
                    childTransform->Position = {0,y};
                    y+=childSize.y;
                }
            } else {
                float maxMargin = layouter->GlobalMax().y - layouter->GlobalDesired().y;
                float fraction = maxMargin > -0.00001f && maxMargin < 0.00001f ? 1 : (size.y - layouter->GlobalDesired().y) / maxMargin;
                if (fraction>1) fraction = 1;
            
                float y = 0;
                for(auto child : object->Children()) {
                    Layouter* childLayouter = child->GetComponent<Layouter>();
                    Sizeable* childSizable = child->GetComponent<Sizeable>();
                    Transform* childTransform = child->GetComponent<Transform>();
                    if (!childLayouter || !childSizable || !childTransform) continue;
                    
                    float desWidth = childLayouter->GlobalDesired().y;
                    float maxWidth = childLayouter->GlobalMax().y;
                    
                    Vector2 childSize = { size.x, desWidth + (maxWidth - desWidth) * fraction };
                    
                    childSizable->Size = childSize;
                    childTransform->Position = {0, y};
                    y+=childSize.y;
                }
            }

        
        
//            if (layouter->GlobalMin().y>=size.y) {
//                float y = 0;
//                for(auto child : object->Children()) {
//                    Layouter* childLayouter = child->GetComponent<Layouter>();
//                    Sizeable* childSizable = child->GetComponent<Sizeable>();
//                    Transform* childTransform = child->GetComponent<Transform>();
//                    if (!childLayouter || !childSizable || !childTransform) continue;
//                    
//                    Vector2 childSize = { size.x, childLayouter->GlobalMin().y };
//                    
//                    childSizable->Size = childSize;
//                    childTransform->Position = {0,y};
//                    y+=childSize.y;
//                }
//            } else if (layouter->GlobalDesired().y>=size.y) {
//                float desiredMargin = layouter->GlobalDesired().y - layouter->GlobalMin().y;
//                float fraction = (size.y - layouter->GlobalMin().y) / desiredMargin;
//                float y = 0;
//                for(auto child : object->Children()) {
//                    Layouter* childLayouter = child->GetComponent<Layouter>();
//                    Sizeable* childSizable = child->GetComponent<Sizeable>();
//                    Transform* childTransform = child->GetComponent<Transform>();
//                    if (!childLayouter || !childSizable || !childTransform) continue;
//                    
//                    float minWidth = childLayouter->GlobalMin().y;
//                    float desWidth = childLayouter->GlobalDesired().y;
//                    
//                    Vector2 childSize = { size.x, minWidth + (desWidth - minWidth) * fraction };
//                    
//                    childSizable->Size = childSize;
//                    childTransform->Position = {0,y};
//                    y+=childSize.y;
//                }
//            } else {
//                float maxMargin = layouter->GlobalMax().y - layouter->GlobalDesired().y;
//                float fraction = maxMargin > -0.00001f && maxMargin < 0.00001f ? 1 : (size.y - layouter->GlobalDesired().y) / maxMargin;
//                
//                
//                float y = 0;
//                for(auto child : object->Children()) {
//                    Layouter* childLayouter = child->GetComponent<Layouter>();
//                    Sizeable* childSizable = child->GetComponent<Sizeable>();
//                    Transform* childTransform = child->GetComponent<Transform>();
//                    if (!childLayouter || !childSizable || !childTransform) continue;
//                    
//                    float desWidth = childLayouter->GlobalDesired().y;
//                    float maxWidth = childLayouter->GlobalMax().y;
//                    
//                    Vector2 childSize = { size.x, desWidth + (maxWidth - desWidth) * fraction };
//                    
//                    childSizable->Size = childSize;
//                    childTransform->Position = {0,y};
//                    y+=childSize.y;
//                }
//                
//            }
        }
        
    }
    
};

*/


class Game : public GameState<Game> {
public:
    GameWorld world;
    GameObject* box;
    GameObject* dragger;
    
    struct Component {
    
        Property<int> Value;
    };
    
    void Initialize() {
        
        
        GameObject* root = world.CreateRoot();
        Gui* gui = root->CreateSystem<Gui>();
        root->CreateSystem<LayoutSystem>();
        
        gui->Setup("images.png", "images.xml", Context().Viewport());
        
        
        
        box = gui->CreateControl(root, "Box", 100, 300);
        box->AddComponent<Draggable>();
        box->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
        box->GetComponent<Layouter>()->SetAll(300);
        
        
        for(int i=0; i<4; i++) {
            GameObject* row = gui->CreateControl(box, "Box");
            row->AddComponent<Layouter>()->Min = {300, 1};
            row->GetComponent<Layouter>()->Max = {300.0f, static_cast<float>((i == 1 ? 60 : 20))};
            row->GetComponent<Layouter>()->Desired = {300.0f, static_cast<float>((i == 1 ? 60 : 20))};
            
            
            row->GetComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Horizontal;
        
            AddTextBox(row, gui, {50, 20}, {50,20});
            AddTextBox(row, gui, {100, 20}, {5000,20});
            //AddTextBox(row, gui, {200, 20}, {300,20});
        }
        
        dragger = gui->CreateControl(root, "Box", 100, 30);
        dragger->AddComponent<Draggable>();

        
        //auto font = gui->CreateFont("Font.fnt", "Font");
        
        //gui->CreateClipper(box, true);
        //auto label = gui->CreateLabel(box, 100, 200, font, "Hej med dig", 50);
        //gui->CreateClipper(box, false);
        
        
        
        
        /*
        GameObject* root = world.CreateRoot();
        
        Selectable* sel1 = 0;
        {
            GameObject* object = root->CreateChild();
            sel1 = object->AddComponent<Selectable>();
            sel1->Selected.Changed.Bind(this, &Game::SelectedChanged, object);
            std::cout << "1. empty = " << (sel1->Selected.Changed.Empty() ? "true" : "false") << std::endl;
            object->Remove();
        }
        world.Update(0);
        Selectable* sel2 = 0;
        {
            GameObject* object = root->CreateChild();
            sel2 = object->AddComponent<Selectable>();
            //sel2->Selected.Changed.Bind(this, &Game::SelectedChanged, object);
            std::cout << "2. empty = " << (sel2->Selected.Changed.Empty() ? "true" : "false") << std::endl;
            object->Remove();
        }
        if (sel1 == sel2) {
            std::cout << "Same" << std::endl;
        }
        
        Component cDefault = Component();
        //cDefault.Value = 123;
        
        Component comp1 = cDefault;
        comp1.Value.Changed.Bind(this, &Game::SelectedChanged, (GameObject*)0);
        
        comp1.Value = cDefault.Value;
        
        comp1 = cDefault;
        {
        
        }
        */
        
    }
    
    void SelectedChanged(GameObject* object) {
        std::cout << "Changed"<< std::endl;
    }
    
    void AddTextBox(GameObject* parent, Gui* gui, Vector2 desired, Vector2 max) {
        GameObject* box = gui->CreateControl(parent, "TextBox");
        box->AddComponent<Layouter>()->Min = {30,2};
        box->GetComponent<Layouter>()->Max = max;
        box->GetComponent<Layouter>()->Desired = desired;
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        Vector3 pos = dragger->GetComponent<Transform>()->Position;
        box->GetComponent<Sizeable>()->Size = { pos.x, pos.y };
        world.Update(dt);
    }
    
    void Render() {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main_tesGui() {
    Engine e;
    e.Start<Game>();
	return 0;
}
