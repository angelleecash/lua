function fun(x)
	coroutine.yield(-1, x)
end

function fun1(x)
	fun(x+1)
	return 3
end
