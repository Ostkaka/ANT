

----------------------------------------------------------------------------------
-- Base processes for the actor manager
----------------------------------------------------------------------------------


----------------------------------------------------------------------------------
-- Manager for the scripted characters in the game
----------------------------------------------------------------------------------
ActorManager = class(nil,
{
	_player = nil,
	_actors = {}
});

function ActorManager:AddPlayer(scriptObject)
	if (self._player ~= nil) then
		print("Adding player when player is active")
	end

	-- add new player
	self._player = scriptObject;

	Utils.DumpObject(EventType)

	-- Tell the engine we added a player
	queueEvent(EventType.EvtData_Set_Controlled_Actor, self._player:getActorId());
end

function ActorManager:AddActor(scriptObject)
	if (self._player ~= nil) then
		print("Adding player when player is active")
	end

	self._actors[actorId] = scriptObject;
	
	Utils.DumpObject(EventType);
end

function ActorManager:RemovePlayer( scriptObject )
	self._player = nil;
end

function ActorManager:GetPlayer()
	return self._player;
end

function ActorManager:GetActorById(actorId)
	local actor = nil;

	if(self._player and self._player:getActorId() == actorId) then
		actor = self._player;
	end

	-- other actors
	if(not actor) then
		actor = self:GetActor(actorId);
	end

	return actor;
end

function ActorManager:GetActor(actorId)
	return self._actors[actorId];
end

function ActorManager:RemoveActor(actorId)
	return self._actors[actorId];
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

function ActorManager._handleSpawnerCollision( actorA. actorB )
	-- handle spawn enemies
	local spawnActor = nil;
	local actor = nil;
	if(actorA.actorType == "Player" and actorB.actorType == "spawnActor") then
		actor = actorA;
		spawnActor = actorB;
	end

	if(actorA.actorType == "spawnActor" and actorB.actorType == "Player") then
		actor = actorBB;
		spawnActor = actorA;
	end

	if(actor ~= nil and spawnActor ~= nil) then
		local pos = spawnActor:getPos();
		local rot = 0;
		pos.x += 10;

		createActor("actors/kamek.xml",pos,rot);  

		-- should also remove spawner?
	end	
end

function ActorManager:OnPhysicsCollision( scriptObject )
	print("Got collide event")

	local actorA = self:GetActorById(scriptObject.actorA)
	local actorB = self:GetActorById(scriptObject.actorB)

	-- handle collision with spawn events
	if(actorA == nill or actorB == nil) then 
		print("Got bad collision data");
		return;
	end

	_handleSpawnerCollision(actorA, actorB);
	
end

function ActorManager:OnPhysicsSeparation( scriptObject )
	print("Got separate event")
end