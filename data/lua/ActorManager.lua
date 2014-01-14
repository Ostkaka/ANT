

----------------------------------------------------------------------------------
-- Base processes for the actor manager
----------------------------------------------------------------------------------


----------------------------------------------------------------------------------
-- Manager for the scripted characters in the game
----------------------------------------------------------------------------------
ActorManager = class(nil,
{
	_player = nil,
	_objects = {}
});

function ActorManager:AddPlayer(scriptObject)
	if (self._player ~= nil) then
		print("Adding player when player is active")
	end

	-- add new player
	self._player = scriptObject;

	print("dfdsgsdgsd   "..self._player:getActorId())
	print("dfdsgsdgsd   event"..EventType.EvtData_Set_Controlled_Actor)
	Utils.DumpObject(EventType)

	-- Tell the engine we added a player
	queueEvent(EventType.EvtData_Set_Controlled_Actor, self._player:getActorId());
end

function ActorManager:RemovePlayer( scriptObject )
	self._player = nil;
end

function ActorManager:GetPlayer()
	return self._player;
end

function ActorManager:CreateCircle()
	local pos = Vec2:Create(); 
	pos.x = 0; 
	pos.y = 200;
	local rot = 0;
	-- Create random pos around origon
	local sphere = createActor("actors\\circle.xml",pos,rot);
end

function ActorManager:RemoveObject(scriptObject)
	local actorId = scriptObject:GetActorId();
	self._objects[actorId] = nil;
end