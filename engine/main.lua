done=false
setScreen(500,500,false,"LuaTest")
loadFont("Font.ttf",28)
a = {}
a[0]=loadImageIndex("shotgun.bmp",WHITE)
for i=1,36 do
	a[i]=rotatePic(a[0],i*10,WHITE)
end 


update()
while not done do
	x,y,l,r=getMouseState()
	angle=getAngle(250,250,x,y)
	angle= math.fmod ((angle/10),36)
	angle=math.floor (angle)
	tmp= angle*10
	drawLine(0,500,500,0,BLACK)
	drawImageIndex(250-picWidth(a[angle])/2,250-picHeight(a[angle])/2,a[angle])
	drawCircle(250,250,18,BLACK)
	update()
	cls(BLACK)
	done=tryQuit() or keyDown(32) or l
end
endP()
