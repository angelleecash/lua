Account = {b = 0}

--use : to save typing self
function Account:withdraw(n)
    self.b = self.b - n 
    return self.b
end

function Account:deposite(n)
    self.b = self.b + n
    return self.b
end

function Account:newAccount(t)
    local a = t or {}
    setmetatable(a, self)
    self.__index = self
    return a
end

o = Account
Account = nil
k = o:newAccount({b=500})
print(k:deposite(100))
print(getmetatable(k).__index.deposite(k, 50))

sa = o:newAccount({b=100})
s = sa:newAccount{limit=1000}

function sa:withdraw(n)
    print("Special account withdrawing " .. n)
    self.b = self.b - n
    return self.b
end

function sa:getLimit()
    return self.limit
end

print(sa:withdraw(10))

print("special account limit:" .. s:getLimit())

function search(t, list)
    for i=1,#list do
        local v = list[i][t]
        if v then return v end
    end
end

function createClass(...)
    local c = {}
    local parent = {...}
    setmetatable(c, {__index=function(t,k) return search(k, parent) end})
    function c:new(t)
        local o = t or {}
        setmetatable(o, c)
        return o
    end
    c.__index = c
    return c
end

Dog = {}
function Dog:woof()
    print("Dog woofing")
end

Apple = {}
function Apple:fall()
    print("Apple falling from the tree")
end

monster = createClass(Dog, Apple)
m = monster:new()
m:fall()
m:woof()
