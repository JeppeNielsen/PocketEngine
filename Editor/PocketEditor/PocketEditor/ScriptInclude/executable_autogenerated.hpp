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
componentToFunction[GameIDHelper::GetComponentID<Homeland::Particle>()] = [](GameObject* object) -> TypeInfo {
Homeland::Particle* component = object->GetComponent<Homeland::Particle>();
TypeInfo typeInfo;
	      typeInfo.name = "Homeland::Particle";
	      typeInfo.AddField(component->rotation, "rotation");
	      typeInfo.AddField(component->x, "x");
	      typeInfo.AddField(component->y, "y");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Homeland::Spring>()] = [](GameObject* object) -> TypeInfo {
Homeland::Spring* component = object->GetComponent<Homeland::Spring>();
TypeInfo typeInfo;
	      typeInfo.name = "Homeland::Spring";
	      typeInfo.AddField(component->rotation, "rotation");
	      typeInfo.AddField(component->x, "x");
	      typeInfo.AddField(component->y, "y");
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
