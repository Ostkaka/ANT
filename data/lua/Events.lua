function OnPhysicsCollision( eventData )
	ActorManagerSingleton:OnPhysicsCollision(eventData);
end

function OnPhysicsSeparation( eventData )
	ActorManagerSingleton:OnPhysicsSeparation(eventData);
end

function RegisterListeners()
	print("Registering listeners")
	if (EventType.EvtData_PhysCollision ~= nil) then
		registerEventListener(EventType.EvtData_PhysCollision, OnPhysicsCollision);
		print("Registering: "..EventType.EvtData_PhysCollision)
	end	

	if (EventType.EvtData_PhysSeparation ~= nil) then
		registerEventListener(EventType.EvtData_PhysSeparation, OnPhysicsSeparation);
		print("Registering: "..EventType.EvtData_PhysSeparation)
	end	
end
