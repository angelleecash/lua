require("boolarray")
--[[
ba = boolarray.new(5)
boolarray.set(ba, 2, true)
print(boolarray.get(ba, 2))
print("size " .. boolarray.size(ba))
]]
--[[
ba = boolarray.new(5)
print(ba:size())
print("before ba[3]" .. tostring(ba:get(3)))
ba:set(3, true)
print("after ba[3]" .. tostring(ba:get(3)))
]]

ba = boolarray.new(10)
print(string.format("before set ba[3]=%s", tostring(ba[3])))

ba[3] = true
print(string.format("after set ba[3]=%s", tostring(ba[3])))

--boolarray.set(io.stdin, 1, false)
--print("ok")
