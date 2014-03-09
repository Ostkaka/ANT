function OnPhysicsCollision( eventData )
	ActorManagerSingleton:OnPhysicsCollision(eventData);
end

function OnPhysicsSeparation( eventData )
	ActorManagerSingleton:OnPhysicsSeparation(eventData);
end

function OnActorJump( eventData )
	ActorManagerSingleton:OnActorJump(eventData);
end

function RegisterListeners()
	print("Registering listeners")
	if (EventType.EvtData_PhysCollision ~= nil) then
		registerEventListener(EventType.EvtData_PhysCollision, OnPhysicsCollision);
	end	

	if (EventType.EvtData_PhysSeparation ~= nil) then
		registerEventListener(EventType.EvtData_PhysSeparation, OnPhysicsSeparation);
	end	

	if (EventType.EvtData_StartJump ~= nil) then
		registerEventListener(EventType.EvtData_StartJump, OnActorJump);
	end	
end
