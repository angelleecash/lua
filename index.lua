t = {}
--setmetatable(t, {__index=function() print("metatable") return function() print("p running") end end})
setmetatable(t, {__index=function() print("metatable") end})


--t.__index = t
--t.__index = function() return function() print("fucking ....") end  end

k = t.p

