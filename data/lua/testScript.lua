print("Loading script");

sParent = nil;
sChild = nil;

TestScriptProcess = class(ScriptProcess,
{
	count = 0;
	finished = false;
});

function TestScriptProcess:onInit()
	print("onInit()");
end

function TestScriptProcess:onUpdate(dt)
	self.count = self.count + 1;
	print("Count: " .. self.count);
	
	if self.count >= 5 then
		self:succeedAndStop();
	end
end

function TestScriptProcess:onSuccess()
	print("success!!");
	self.finished = true;
end

function TestScriptProcess:onFail()
	print("Something failed");
end

function TestProcess()
	sParent = TestScriptProcess:create({frequency = 1});
	sChild = TestScriptProcess:create({frequency = 0.5});
	sParent:attachChild(sChild);
	attachProcess(sParent);
end

--RegisterListener();

print("Script loaded");