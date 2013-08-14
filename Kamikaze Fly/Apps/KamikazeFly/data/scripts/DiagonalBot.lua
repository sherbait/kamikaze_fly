timer = 0
movingUp = true

function update()
	timer = timer - 1
	if timer < 0 then
		timer = 30
		if movingUp then
			setVelocity(10,10)
			movingUp = false
			setState('RIGHT')
		else
			setVelocity(-10,-10)
			movingUp = true
			setState('LEFT')
		end
	end
end