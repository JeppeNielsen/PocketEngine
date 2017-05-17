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
componentToFunction[GameIDHelper::GetComponentID<Particles::Particle>()] = [](GameObject* object) -> TypeInfo {
Particles::Particle* component = object->GetComponent<Particles::Particle>();
TypeInfo typeInfo;
	      typeInfo.name = "Particles::Particle";
	      typeInfo.AddField(component->rotation, "rotation");
	      typeInfo.AddField(component->x, "x");
	      typeInfo.AddField(component->y, "y");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Particles::Spring>()] = [](GameObject* object) -> TypeInfo {
Particles::Spring* component = object->GetComponent<Particles::Spring>();
TypeInfo typeInfo;
	      typeInfo.name = "Particles::Spring";
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
