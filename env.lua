function setfield(f,v)
    local t = _G
    for w,d in string.gmatch(f, "([%w_]+)(%.?)")do
        if d == "." then
            t[w] = {}
            t = t[w]
        else
            t[w] = v
        end
    end
end
--[[
setfield("t.x.y.z.q.w", 4)
print(t)
print(t.x)
print(t.x.y)
print(t.x.y.z)
print(t.x.y.z.q)
print(t.x.y.z.q.w)
--t.x.y = 3
]]
function traverse(t)
    for k,v in pairs(t) do
        if(type(v) ~= "table" and type(v) ~= "function") then
            print(k .. ":" .. v)
        end
    end
end

function mypackage(env)
    local _ENV = env
    local pac = {}
    function pac.fun1()
        print()
    end
    return pac 
end
print(_ENV.print)
print(_G.print)
env = {print=function() _ENV.print("i won't print") end}
pac = mypackage(env)

pac.fun1()

