GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<Collidable>()] = [](GameObject* object) -> TypeInfo {
Collidable* component = object->GetComponent<Collidable>();
TypeInfo typeInfo;
	      typeInfo.name = "Collidable";
	      typeInfo.AddField(component->radius, "radius");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Controller>()] = [](GameObject* object) -> TypeInfo {
Controller* component = object->GetComponent<Controller>();
TypeInfo typeInfo;
	      typeInfo.name = "Controller";
	      typeInfo.AddField(component->downKey, "downKey");
	      typeInfo.AddField(component->speed, "speed");
	      typeInfo.AddField(component->upKey, "upKey");
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
