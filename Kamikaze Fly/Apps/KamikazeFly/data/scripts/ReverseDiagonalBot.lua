timer = 0
movingUp = true
speed = 10

function update()
	timer = timer - 1
	if timer < 0 then
		timer = 30
		if movingUp then
			setVelocity(-speed,-speed)
			movingUp = false
			setState('LEFT')
		else
			setVelocity(speed,speed)
			movingUp = true
			setState('RIGHT')
		end
	end
end