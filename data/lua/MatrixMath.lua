
-- This requires that Preinit.lua is loaded first. Otherwise the class macro won't work!

-----------------------------------------------------------------------------------------------------------------------
-- Vec2 class
-----------------------------------------------------------------------------------------------------------------------
Vec2 = class(nil, 
{
	x = 0,
	y = 0,
	__operators = {},
});

function Vec2:Length()
	return math.sqrt((self.x * self.x) + (self.y * self.y);
end

function Vec2:Length2()
	return ((self.x * self.x) + (self.y * self.y);
end

function Vec2:Normalize()
	local len = self:Length();
	self.x = self.x / len;
	self.y = self.y / len;
end

function Vec2.__operators.__add(left, right)
	local temp = Vec2:Create();
	if (type(right) == "number") then
		temp.x = left.x + right;
		temp.y = left.y + right;
	else
		temp.x = left.x + right.x;
		temp.y = left.y + right.y;
	end
	return temp;
end

function Vec2.__operators.__sub(left, right)
	local temp = Vec2:Create();
	if (type(right) == "number") then
		temp.x = left.x - right;
		temp.y = left.y - right;
	else
		temp.x = left.x - right.x;
		temp.y = left.y - right.y;
	end
	return temp;
end

function Vec2.__operators.__mul(left, right)
	local temp = Vec2:Create();
	if (type(right) == "number") then
		temp.x = left.x * right;
		temp.y = left.y * right;
	else
		temp.x = left.x * right.x;
		temp.y = left.y * right.y;
	end
	return temp;
end

function Vec2.__operators.__div(left, right)
	local temp = Vec2:Create();
	if (type(right) == "number") then
		temp.x = left.x / right;
		temp.y = left.y / right;
	else
		temp.x = left.x / right.x;
		temp.y = left.y / right.y;
	end
	return temp;
end
