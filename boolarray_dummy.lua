require("boolarray")
--[[
ba = boolarray.new(5)
boolarray.set(ba, 2, true)
print(boolarray.get(ba, 2))
print("size " .. boolarray.size(ba))
]]

ba = boolarray.new(5)
print("=========================================" .. tostring(getmetatable(ba)))
print(ba:size())
print("before ba[3]" .. tostring(ba:get(3)))
ba:set(3, true)
print("after ba[3]" .. tostring(ba:get(3)))



--boolarray.set(io.stdin, 1, false)
--print("ok")
