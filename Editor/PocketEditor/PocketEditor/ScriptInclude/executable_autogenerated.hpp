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
componentToFunction[GameIDHelper::GetComponentID<Jumper>()] = [](GameObject* object) -> TypeInfo {
Jumper* component = object->GetComponent<Jumper>();
TypeInfo typeInfo;
	      typeInfo.name = "Jumper";
	      typeInfo.AddField(component->height, "height");
	      typeInfo.AddField(component->speed, "speed");
	      typeInfo.AddField(component->time, "time");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
