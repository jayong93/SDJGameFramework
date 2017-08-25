player = GetObjectByName("TestObj")
player2 = GetObjectByName("TestObj2")
print(player.handle)
print(player2.handle)
print(player:Position())
player:Move(5,5,5)
print(player:Position())

t = {1,2,3,4,x=3,y=5}