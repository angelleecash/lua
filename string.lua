f = io.output("./tmp", "rw")
--io.write(string.rep("f", 2^20))

print(string.upper("abCD"))
print(string.lower("FUCK"))
print(#"abc")

a = "abc"
print(a:sub(1,3))
c = {string.byte("abcde", 1, -1)}
print(string.char(unpack(c)))
