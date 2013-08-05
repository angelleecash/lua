t = split("abc,def,ghi", ",");
for k,v in pairs(t) do
	print(k .. ":" .. v)
end

s = upper("my fucking lower case string")
print(s)

s = upper("another lower case string")
print(s)

