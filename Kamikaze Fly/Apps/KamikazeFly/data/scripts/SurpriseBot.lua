seenPlayer = false
startingBelowPlayer = false
sensorRadius = 500
speed = 15

function update()
	if not seenPlayer then
		if distanceToPlayer() < sensorRadius then
			seenPlayer = true
			if getY() > playerY() then
				startingBelowPlayer = true
			end
		end
	else
		if startingBelowPlayer then
			setVelocity(0,-speed)
		else
			setVelocity(0,speed)
		end
	end
end