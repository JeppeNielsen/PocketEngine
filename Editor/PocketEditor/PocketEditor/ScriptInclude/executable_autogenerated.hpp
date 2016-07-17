GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<ColorClicker>()] = [](GameObject* object) -> TypeInfo {
ColorClicker* component = object->GetComponent<ColorClicker>();
TypeInfo typeInfo;
	      typeInfo.name = "ColorClicker";
	      typeInfo.AddField(component->useBlue, "useBlue");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Rotator>()] = [](GameObject* object) -> TypeInfo {
Rotator* component = object->GetComponent<Rotator>();
TypeInfo typeInfo;
	      typeInfo.name = "Rotator";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Velocity>()] = [](GameObject* object) -> TypeInfo {
Velocity* component = object->GetComponent<Velocity>();
TypeInfo typeInfo;
	      typeInfo.name = "Velocity";
	      typeInfo.AddField(component->Size, "Size");
	      typeInfo.AddField(component->children, "children");
	      typeInfo.AddField(component->coords, "coords");
	      typeInfo.AddField(component->name, "name");
	      typeInfo.AddField(component->timer, "timer");
	      typeInfo.AddField(component->velocity, "velocity");
	      typeInfo.AddField(component->wobble, "wobble");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
