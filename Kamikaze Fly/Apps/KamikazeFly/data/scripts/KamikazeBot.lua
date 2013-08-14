seenPlayer = false
sensorRadius = 1500
speed = 7

function update()
	if not seenPlayer then
		if distanceToPlayer() < sensorRadius then
			seenPlayer = true
		end
	else
		moveToPlayer(speed)
	end
end