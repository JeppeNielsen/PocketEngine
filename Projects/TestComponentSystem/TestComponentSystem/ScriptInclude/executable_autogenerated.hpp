GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<Position>()] = [](GameObject* object) -> TypeInfo {
Position* component = object->GetComponent<Position>();
TypeInfo typeInfo;
	      typeInfo.name = "Position";
	      typeInfo.AddField(component->position, "position");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Velocity>()] = [](GameObject* object) -> TypeInfo {
Velocity* component = object->GetComponent<Velocity>();
TypeInfo typeInfo;
	      typeInfo.name = "Velocity";
	      typeInfo.AddField(component->velocity, "velocity");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
