--[[
debug.sethook(print, "l")
function fun()
    printf("I'm almight fun")
end

t = debug.getinfo(fun)

for k,v in pairs(t) do
    print(k..":"..tostring(v))
end
]]

c = {}
n = {}
local function hook()
    local f = debug.getinfo(2, "f").func
    if c[f] == nil then
        c[f] = 1
        n[f] = debug.getinfo(2, "Sn")
    else
        c[f] = c[f] + 1
    end
end

local f = loadfile("./oop.lua")
debug.sethook(hook, "c")
f()
debug.sethook()

for f,fc in pairs(c) do
    fn = "unknown"
    func = n[f]
    if func then
        if string.upper(func.what) == "C" then
            fn = "C function (" .. (func.name or func.namewhat or "unknown c function") .. ")"
        else
            fn = string.format("%s,%s", func.short_src, func.linedefined)
            if func.namewhat ~= "" then 
                fn = string.format("%s %s", fn, func.name)    
            end
        end
    end
    print(fn .. " called [" .. fc .. "] time(s)")
end
