local io = require "io"

for k in pairs(io) do
    print(k)
end
print(package.path)
print(package.cpath)
--myrequire("io")

