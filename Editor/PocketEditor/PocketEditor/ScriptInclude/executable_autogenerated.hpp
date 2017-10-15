GameWorld::OnGetTypeInfo = [] (int componentID, GameObject::ComponentInfo& info) {
 static std::map<int, std::function<TypeInfo(GameObject*)>> componentToFunction; 
 if (!componentToFunction.empty()) {
componentToFunction[GameIDHelper::GetComponentID<Road>()] = [](GameObject* object) -> TypeInfo {
Road* component = object->GetComponent<Road>();
TypeInfo typeInfo;
	      typeInfo.name = "Road";
	      typeInfo.AddField(component->Origin, "Origin");
	      typeInfo.AddField(component->positions, "positions");
	      typeInfo.AddField(component->rotations, "rotations");
	      typeInfo.AddField(component->widths, "widths");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<RoadGenerator>()] = [](GameObject* object) -> TypeInfo {
RoadGenerator* component = object->GetComponent<RoadGenerator>();
TypeInfo typeInfo;
	      typeInfo.name = "RoadGenerator";
	      typeInfo.AddField(component->frequency, "frequency");
	      typeInfo.AddField(component->time, "time");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<RoadMovement>()] = [](GameObject* object) -> TypeInfo {
RoadMovement* component = object->GetComponent<RoadMovement>();
TypeInfo typeInfo;
	      typeInfo.name = "RoadMovement";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<RoadOrigin>()] = [](GameObject* object) -> TypeInfo {
RoadOrigin* component = object->GetComponent<RoadOrigin>();
TypeInfo typeInfo;
	      typeInfo.name = "RoadOrigin";
	      typeInfo.AddField(component->variable, "variable");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<RoadPosition>()] = [](GameObject* object) -> TypeInfo {
RoadPosition* component = object->GetComponent<RoadPosition>();
TypeInfo typeInfo;
	      typeInfo.name = "RoadPosition";
	      typeInfo.AddField(component->Position, "Position");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<RoadRenderer>()] = [](GameObject* object) -> TypeInfo {
RoadRenderer* component = object->GetComponent<RoadRenderer>();
TypeInfo typeInfo;
	      typeInfo.name = "RoadRenderer";
	      typeInfo.AddField(component->Depth, "Depth");
	      typeInfo.AddField(component->origin, "origin");
	      typeInfo.AddField(component->renderOffset, "renderOffset");
	      typeInfo.AddField(component->segments, "segments");
	      typeInfo.AddField(component->vScale, "vScale");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<Rotatable>()] = [](GameObject* object) -> TypeInfo {
Rotatable* component = object->GetComponent<Rotatable>();
TypeInfo typeInfo;
	      typeInfo.name = "Rotatable";
	      typeInfo.AddField(component->angularVelocity, "angularVelocity");
return typeInfo;
};
componentToFunction[GameIDHelper::GetComponentID<TransformFollower>()] = [](GameObject* object) -> TypeInfo {
TransformFollower* component = object->GetComponent<TransformFollower>();
TypeInfo typeInfo;
	      typeInfo.name = "TransformFollower";
	      typeInfo.AddField(component->lookAt, "lookAt");
	      typeInfo.AddField(component->position, "position");
	      typeInfo.AddField(component->smoothness, "smoothness");
	      typeInfo.AddField(component->target, "target");
return typeInfo;
};
   }
info.getTypeInfo = componentToFunction[componentID];
};
