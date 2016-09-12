GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<Blinker>()] = [](GameObject* object) -> TypeInfo {
Blinker* component = object->GetComponent<Blinker>();
TypeInfo typeInfo;
	      typeInfo.name = "Blinker";
	      typeInfo.AddField(component->speed, "speed");
	      typeInfo.AddField(component->time, "time");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<ColorClicker>()] = [](GameObject* object) -> TypeInfo {
ColorClicker* component = object->GetComponent<ColorClicker>();
TypeInfo typeInfo;
	      typeInfo.name = "ColorClicker";
	      typeInfo.AddField(component->useBlue, "useBlue");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Mover>()] = [](GameObject* object) -> TypeInfo {
Mover* component = object->GetComponent<Mover>();
TypeInfo typeInfo;
	      typeInfo.name = "Mover";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<ReverseClicker>()] = [](GameObject* object) -> TypeInfo {
ReverseClicker* component = object->GetComponent<ReverseClicker>();
TypeInfo typeInfo;
	      typeInfo.name = "ReverseClicker";
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Rotator>()] = [](GameObject* object) -> TypeInfo {
Rotator* component = object->GetComponent<Rotator>();
TypeInfo typeInfo;
	      typeInfo.name = "Rotator";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Turner>()] = [](GameObject* object) -> TypeInfo {
Turner* component = object->GetComponent<Turner>();
TypeInfo typeInfo;
	      typeInfo.name = "Turner";
	      typeInfo.AddField(component->reverse, "reverse");
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
