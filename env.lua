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

setfield("t.x.y.z.q.w", 4)
print(t)
print(t.x)
print(t.x.y)
print(t.x.y.z)
print(t.x.y.z.q)
print(t.x.y.z.q.w)
--t.x.y = 3

