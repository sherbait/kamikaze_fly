timer = 0
movingUp = true

function update()
	timer = timer - 1
	if timer < 0 then
		timer = 30
		if movingUp then
			setVelocity(0,10)
			movingUp = false
		else
			setVelocity(0,-10)
			movingUp = true
		end
	end
end