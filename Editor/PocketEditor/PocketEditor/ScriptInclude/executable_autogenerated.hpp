GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<Controller>()] = [](GameObject* object) -> TypeInfo {
Controller* component = object->GetComponent<Controller>();
TypeInfo typeInfo;
	      typeInfo.name = "Controller";
	      typeInfo.AddField(component->downKey, "downKey");
	      typeInfo.AddField(component->upKey, "upKey");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
