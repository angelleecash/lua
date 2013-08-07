require("xml")


count = 0

callbacks = {
				startElement = function(parser, tagname)
								   --print("*************************************start element callback")
							       io.write(string.format("+ %s %s\n", string.rep(" ", count), tagname))
								   count = count + 1
							   end,
				endElement   = function(parser, tagname)
							       --print("*************************************end element callback")
								   count = count - 1
							       io.write(string.format("- %s %s\n", string.rep(" ", count), tagname))
							   end
			}

x = xml.new(callbacks)

for l in io.lines() do
	x:parse(l)
	--x:parse("\n")
end
--x:parse()
--x:close()

