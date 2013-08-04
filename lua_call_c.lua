--print(tostring(io.open))
require("mylib")
t = mylib.ls("/home/chenliang")

--t = _G 
--t = package.loaded
for k,v in pairs(t) do
	print(v)
end
