print("Loading script");

TestScriptProcess = class(ScriptProcess,
{
	count = 0;
});

function TestScriptProcess:onInit()
	print("onInit()");
end

function TestScriptProcess:onUpdate(dt)
	self.count = self.count + 1;
	print("Count: " .. self.count);
	
	if self.count >= 5 then
		self:succeed();
	end
end

function TestScriptProcess:onSuccess()
	print("success!!");
end

function TestScriptProcess:onFail()
	print("Something failed");
end

function TestProcess()
	parent = TestScriptProcess:create({frequency = 1});
	child = TestScriptProcess:create({frequency = 0.5});
	parent:attachChild(child);
	attachProcess(parent);
end

--RegisterListener();

print("Script loaded");