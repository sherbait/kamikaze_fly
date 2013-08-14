function update()
	if(playerX() > getX() + 32) then
		setState("RIGHT")
	else
		setState("LEFT")
	end
end