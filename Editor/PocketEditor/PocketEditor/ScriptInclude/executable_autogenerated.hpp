GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<RectCollider>()] = [](GameObject* object) -> TypeInfo {
RectCollider* component = object->GetComponent<RectCollider>();
TypeInfo typeInfo;
	      typeInfo.name = "RectCollider";
	      typeInfo.AddField(component->collisions, "collisions");
	      typeInfo.AddField(component->previousCollisions, "previousCollisions");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Rotator>()] = [](GameObject* object) -> TypeInfo {
Rotator* component = object->GetComponent<Rotator>();
TypeInfo typeInfo;
	      typeInfo.name = "Rotator";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<TransformController>()] = [](GameObject* object) -> TypeInfo {
TransformController* component = object->GetComponent<TransformController>();
TypeInfo typeInfo;
	      typeInfo.name = "TransformController";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
