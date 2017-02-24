GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<RotatorScript>()] = [](GameObject* object) -> TypeInfo {
RotatorScript* component = object->GetComponent<RotatorScript>();
TypeInfo typeInfo;
	      typeInfo.name = "RotatorScript";
	      typeInfo.AddField(component->angularSpeed, "angularSpeed");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
