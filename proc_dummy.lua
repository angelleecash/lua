require("proc")
sender = [[
while true do
	proc.send("c1", "first", "second", "third")
end
]]
receiver = [[
while true do
	proc.receive("c1")
end
]]
proc.start(sender)
proc.start(receiver);

while true do
end
