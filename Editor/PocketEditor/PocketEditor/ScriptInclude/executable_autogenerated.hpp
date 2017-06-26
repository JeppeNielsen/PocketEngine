GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<KillTrigger>()] = [](GameObject* object) -> TypeInfo {
KillTrigger* component = object->GetComponent<KillTrigger>();
TypeInfo typeInfo;
	      typeInfo.name = "KillTrigger";
	      typeInfo.AddField(component->variable, "variable");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<RectCollider>()] = [](GameObject* object) -> TypeInfo {
RectCollider* component = object->GetComponent<RectCollider>();
TypeInfo typeInfo;
	      typeInfo.name = "RectCollider";
	      typeInfo.AddField(component->Enter, "Enter");
	      typeInfo.AddField(component->Exit, "Exit");
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
componentToFunction[GameIDHelper::GetComponentID<Score>()] = [](GameObject* object) -> TypeInfo {
Score* component = object->GetComponent<Score>();
TypeInfo typeInfo;
	      typeInfo.name = "Score";
	      typeInfo.AddField(component->score, "score");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<ScoreTrigger>()] = [](GameObject* object) -> TypeInfo {
ScoreTrigger* component = object->GetComponent<ScoreTrigger>();
TypeInfo typeInfo;
	      typeInfo.name = "ScoreTrigger";
	      typeInfo.AddField(component->scoreToAdd, "scoreToAdd");
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
