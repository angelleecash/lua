width = 1024
height = 768
background = {red=100, green=200, blue=300}

function fun()
	print(string.rep("*", 40))
	print(string.format("width=%d, height=%d", width, height))
	for k,v in pairs(background) do
		print(k .. "=" .. v)
	end

	print(string.rep("*", 40))
end

function fun2(a,b)
	print(string.format("fun2 being called with %d, %d", a, b))
	return 3,4
end
