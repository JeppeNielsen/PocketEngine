GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<Blinker>()] = [](GameObject* object) -> TypeInfo {
Blinker* component = object->GetComponent<Blinker>();
TypeInfo typeInfo;
	      typeInfo.name = "Blinker";
	      typeInfo.AddField(component->frequency, "frequency");
	      typeInfo.AddField(component->time, "time");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Rotator>()] = [](GameObject* object) -> TypeInfo {
Rotator* component = object->GetComponent<Rotator>();
TypeInfo typeInfo;
	      typeInfo.name = "Rotator";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
