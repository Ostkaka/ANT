print("Loading script");

TestScriptProcess = class(ScriptProcess,
{
	count = 0;
});

function TestScriptProcess:onInit()
	print("OnInit()");
end

function TestScriptProcess:onUpdate(deltaMs)
	self.count = self.count + 1;
	print("Count: " .. self.count);
	
	if self.count >= 5 then
		self:Succeed();
	end
end

function TestScriptProcess:onSuccess()
	print("Success!!");
end

function TestScriptProcess:onFail()
	print("Something failed");
end


function TestProcess()
	parent = TestScriptProcess:Create({frequency = 1});
	child = TestScriptProcess:Create({frequency = 0.5});
	parent:AttachChild(child);
	AttachProcess(parent);
end

--RegisterListener();

print("Script loaded");