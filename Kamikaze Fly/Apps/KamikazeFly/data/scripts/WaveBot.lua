vTimer = 0
hTimer = 0
movingUp = true
movingRight = true

function update()
	vTimer = vTimer - 1
	hTimer = hTimer - 1

	if vTimer < 0 then
		vTimer = 20
		if movingUp then
			setVelocity(getVx(),10)
			movingUp = false
		else
			setVelocity(getVx(),-10)
			movingUp = true
		end
	end
	
	if hTimer < 0 then
		hTimer = 90
		if movingRight then
			setVelocity(10,getVy())
			movingRight = false
		else
			setVelocity(-10,getVy())
			movingRight = true
		end
	end

	if movingRight then
		setState('LEFT')
	else
		setState('RIGHT')
	end
end