a = {}
b = {__mode="k"}
setmetatable(a,b)

k = {}

a[k] = 1

k = {}
a[k] = 2

collectgarbage()

for k,v in pairs(a) do
    print(v)
end

