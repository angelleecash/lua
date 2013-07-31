sm = getmetatable("fuck")

--need to format the table output with proper ident
function ite(sm)
    for k,v in pairs(sm)
    do
        if(type(v) == "table") then
            ite(v)        
        elseif type(v) == "function" then
            print(k .. " is a function")
        else
            print(k .. ":" .. v)
        end
    end
    
end

set = {}
setmeta = {}


function set.new(t)
    local _set = {}
    setmetatable(_set, setmeta)
    for k,v in pairs(t) do _set[k] = true end
    return _set
end

function set.union(a, b)
    local _set = set.new{}
    for k,v in pairs(a) do _set[k] = true end
    for k,v in pairs(b) do _set[k] = true end
    return _set
end

function set.intersection(a,b)
    local _set = set.new{}
    for k,v in pairs(a) do
        if b[k] then _set[k] = v end
    end
    return _set
end

function set.tostring(a)
    local _s = {}
    for k,v in pairs(a) do
        _s[#_s+1] = k
    end
    return "{" .. table.concat(_s, ",") .. "}"
end

function set.size(a)
    local i = 0
    for k,v in pairs(a) do
        i = i + 1
    end
    return i
end

setmeta.__add = set.union
setmeta.__le = function (a, b)
                  for k,v in pairs(a) do
                      if not b[k] then return false end
                  end 
                  return true
               end  
setmeta.__eq = function (a, b)
                  if set.size(a) ~= set.size(b) then return false end
                  for k,v in pairs(a) do
                      if not b[k] then return false end
                  end 
                  return true
               end  
setmeta.__lt = function (a, b)
                  if set.size(a) >= set.size(b) then return false end
                  for k,v in pairs(a) do
                      if not b[k] then return false end
                  end 
                  return true
               end  


a = set.new{a=true, b=true, c=true}
b = set.new{b=true, c=true, d=true, e=true}
c = set.new(a)
d = set.new{a=true}

print("a=" .. set.tostring(a))
print("b=" .. set.tostring(b))
print("c=" .. set.tostring(c))
print("d=" .. set.tostring(d))

print("a union b" .. set.tostring(set.union(a,b)))
print("a intersection b" .. set.tostring(set.intersection(a,b)))

print("a+b=" .. set.tostring(a+b))
print("a <= b " .. tostring((a <= b)))
print("a == c " .. tostring(a == c))
--print("d < a " .. tostring(d < a))
