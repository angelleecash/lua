require("boolarray")

ba = boolarray.new(5)
boolarray.set(ba, 2, true)
print(boolarray.get(ba, 2))
print("size " .. boolarray.size(ba))
