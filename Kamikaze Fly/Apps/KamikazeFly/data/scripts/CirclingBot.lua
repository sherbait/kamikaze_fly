timer = 0
state = 1
speed = 7

function update()
	timer = timer - 1
	if timer < 0 then
		timer = 20
		if state == 1 then
			setVelocity(speed,speed)
			setState('RIGHT')
			state = 2
		elseif state == 2 then
			setVelocity(speed,-speed)
			setState('RIGHT')
			state = 3
		elseif state == 3 then
			setVelocity(-speed,-speed)
			setState('LEFT')
			state = 4
		elseif state == 4 then
			setVelocity(-speed,speed)
			setState('LEFT')
			state = 1
		end
	end
end